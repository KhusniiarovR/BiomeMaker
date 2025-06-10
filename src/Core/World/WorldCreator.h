#ifndef WORLDCREATOR_H
#define WORLDCREATOR_H

#include <iostream>
#include <utility>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <fstream>
#include <string>
#include <filesystem>
#include <random>
#include "Constants/WorldConst.h"

class WorldCreator {
private:
    static constexpr int extraCentersInner = 2;
    static constexpr int extraCentersOuter = 5;

    static constexpr int innerRadius = 200;
    static constexpr int ringWidth = 30;
    static constexpr int outerRadius = innerRadius + ringWidth;

// todo configure world size
// merge biomes here and biome.h

    std::string WORLDNAME;

    const char errorBiome = 'U';
    const char centerBiome = 'F';
    const std::vector<char> innerBiomes = { 'O', 'W', 'S', 'V' };
    const std::vector<char> outerBiomes = { 'G', 'B', 'C', 'M' };
    const char ringBiome = 'E';

    struct BiomeCenter {
        int x, y;
        char type;
    };

public:
    WorldCreator() = default;

    ~WorldCreator() = default;

    void Generate(int seed, std::string worldName) {
        std::mt19937 rng(seed);
        WORLDNAME = std::move(worldName);

        std::uniform_int_distribution<int> dist(0, WORLD_SIZE - 1);
        std::vector<BiomeCenter> centers;

        centers.push_back({ WORLD_SIZE / 2, WORLD_SIZE / 2, centerBiome });

        for (char b : innerBiomes) {
            int angle = dist(rng) % 360;
            int r = innerRadius * 2 / 3 + (dist(rng) % (innerRadius / 3));
            int cx = WORLD_SIZE / 2 + static_cast<int>(r * std::cos(angle));
            int cy = WORLD_SIZE / 2 + static_cast<int>(r * std::sin(angle));
            centers.push_back({ cx, cy, b });
        }

        for (char b : outerBiomes) {
            int angle = dist(rng) % 360;
            int r = outerRadius + 30 + (dist(rng) % 50);
            int cx = WORLD_SIZE / 2 + static_cast<int>(r * std::cos(angle));
            int cy = WORLD_SIZE / 2 + static_cast<int>(r * std::sin(angle));
            centers.push_back({ cx, cy, b });
        }

        for (int i = 0; i < extraCentersInner; ++i) {
            char b = innerBiomes[dist(rng) % innerBiomes.size()];
            int r = innerRadius / 2 + (dist(rng) % (innerRadius / 2));
            int angle = dist(rng) % 360;
            int cx = WORLD_SIZE / 2 + static_cast<int>(r * std::cos(angle));
            int cy = WORLD_SIZE / 2 + static_cast<int>(r * std::sin(angle));
            centers.push_back({ cx, cy, b });
        }

        for (int i = 0; i < extraCentersOuter; ++i) {
            char b = outerBiomes[dist(rng) % outerBiomes.size()];
            int r = outerRadius + 40 + (dist(rng) % 50);
            int angle = dist(rng) % 360;
            int cx = WORLD_SIZE / 2 + static_cast<int>(r * std::cos(angle));
            int cy = WORLD_SIZE / 2 + static_cast<int>(r * std::sin(angle));
            centers.push_back({ cx, cy, b });
        }

        std::vector<std::vector<char>> map(WORLD_SIZE, std::vector<char>(WORLD_SIZE, errorBiome));

        for (int y = 0; y < WORLD_SIZE; ++y) {
            for (int x = 0; x < WORLD_SIZE; ++x) {
                int dx = x - WORLD_SIZE / 2;
                int dy = y - WORLD_SIZE / 2;
                int r = static_cast<int>(std::sqrt(dx * dx + dy * dy));

                char biome = errorBiome;

                if (r >= innerRadius && r <= outerRadius) {
                    biome = ringBiome;
                }
                else {
                    int minDist = std::numeric_limits<int>::max();

                    for (const auto& center : centers) {
                        int cx = center.x - WORLD_SIZE / 2;
                        int cy = center.y - WORLD_SIZE / 2;
                        int centerR = static_cast<int>(std::sqrt(cx * cx + cy * cy));

                        bool sameZone =
                            (r < innerRadius && centerR < innerRadius) ||
                            (r > outerRadius && centerR > outerRadius) ||
                            (r <= innerRadius && center.type == 'F');

                        int d2 = sameZone ? dist2(x, y, center.x, center.y) : std::numeric_limits<int>::max();

                        if (d2 < minDist) {
                            minDist = d2;
                            biome = center.type;
                        }
                    }
                }

                map[y][x] = biome;
            }
        }

        save_world_rle(map);
    }

private:
    static int dist2(int x1, int y1, int x2, int y2) {
        return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
    }

    void save_world_rle(const std::vector<std::vector<char>>& world) {
        if (!std::filesystem::exists("saves/")) {
            std::cerr << "Recreated saves folder";
            std::filesystem::create_directory("saves");
        }

        std::filesystem::create_directory("saves/" + WORLDNAME);
        std::string filename = "saves/" + WORLDNAME + "/world.dat";
        std::ofstream out(filename, std::ios::binary);

        if (!out.is_open()) {
            std::cerr << "Error: can't open world.dat\n";
            return;
        }

        int total_chunks = NUM_CHUNKS * NUM_CHUNKS;
        std::vector<ChunkHeader> headers(total_chunks);
        out.seekp(total_chunks * sizeof(ChunkHeader), std::ios::beg);

        for (int cy = 0; cy < NUM_CHUNKS; ++cy) {
            for (int cx = 0; cx < NUM_CHUNKS; ++cx) {
                int startX = cx * CHUNK_SIZE;
                int startY = cy * CHUNK_SIZE;

                std::streampos chunk_start = out.tellp();
                char current = world[startY][startX];
                unsigned char count = 1;

                for (int y = 0; y < CHUNK_SIZE; ++y) {
                    for (int x = 0; x < CHUNK_SIZE; ++x) {
                        if (x == 0 && y == 0) continue;
                        char next = world[startY + y][startX + x];
                        if (next == current && count < 255) {
                            count++;
                        } else {
                            out.write(reinterpret_cast<char*>(&count), 1);
                            out.write(&current, 1);
                            current = next;
                            count = 1;
                        }
                    }
                }

                out.write(reinterpret_cast<char*>(&count), 1);
                out.write(&current, 1);

                std::streampos chunk_end = out.tellp();
                uint32_t offset = static_cast<uint32_t>(chunk_start);
                uint32_t size = static_cast<uint32_t>(chunk_end - chunk_start);
                headers[cy * NUM_CHUNKS + cx] = { offset, size };
            }
        }

        out.seekp(0, std::ios::beg);
        for (const auto& header : headers) {
            out.write(reinterpret_cast<const char*>(&header), sizeof(ChunkHeader));
        }

        out.close();
    }
};


#endif //WORLDCREATOR_H

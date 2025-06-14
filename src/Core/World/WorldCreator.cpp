#include "WorldCreator.h"
#include <iostream>
#include <utility>
#include <cmath>
#include <limits>
#include <fstream>
#include <random>
#include "Constants/WorldConst.h"
#include "Biome.h"

WorldCreator::WorldCreator() = default;

WorldCreator::~WorldCreator() = default;

void WorldCreator::generate() {
    generate(getName());
}

void WorldCreator::generate(std::string worldName) {
    std::random_device rd;
    int seed = static_cast<int>(rd());
    generate(seed, std::move(worldName));
}

void WorldCreator::generate(int seed, std::string worldName) {
    {
        std::mt19937 rng(seed);
        this->worldName = std::move(worldName);

        std::uniform_int_distribution<int> dist(0, worldSize - 1);
        std::vector<BiomeCenter> centers;

        centers.push_back({ worldSize / 2, worldSize / 2, centerBiome });

        for (char b : innerBiomes) {
            int angle = dist(rng) % 360;
            int r = innerRadius * 2 / 3 + (dist(rng) % (innerRadius / 3));
            int cx = worldSize / 2 + static_cast<int>(r * std::cos(angle));
            int cy = worldSize / 2 + static_cast<int>(r * std::sin(angle));
            centers.push_back({ cx, cy, b });
        }

        for (char b : outerBiomes) {
            int angle = dist(rng) % 360;
            int r = outerRadius + 30 + (dist(rng) % 50);
            int cx = worldSize / 2 + static_cast<int>(r * std::cos(angle));
            int cy = worldSize / 2 + static_cast<int>(r * std::sin(angle));
            centers.push_back({ cx, cy, b });
        }

        for (int i = 0; i < extraCentersInner; ++i) {
            char b = innerBiomes[dist(rng) % innerBiomes.size()];
            int r = innerRadius / 2 + (dist(rng) % (innerRadius / 2));
            int angle = dist(rng) % 360;
            int cx = worldSize / 2 + static_cast<int>(r * std::cos(angle));
            int cy = worldSize / 2 + static_cast<int>(r * std::sin(angle));
            centers.push_back({ cx, cy, b });
        }

        for (int i = 0; i < extraCentersOuter; ++i) {
            char b = outerBiomes[dist(rng) % outerBiomes.size()];
            int r = outerRadius + 40 + (dist(rng) % 50);
            int angle = dist(rng) % 360;
            int cx = worldSize / 2 + static_cast<int>(r * std::cos(angle));
            int cy = worldSize / 2 + static_cast<int>(r * std::sin(angle));
            centers.push_back({ cx, cy, b });
        }

        std::vector<std::vector<char>> map(worldSize, std::vector<char>(worldSize, errorBiome));

        for (int y = 0; y < worldSize; ++y) {
            for (int x = 0; x < worldSize; ++x) {
                int dx = x - worldSize / 2;
                int dy = y - worldSize / 2;
                int r = static_cast<int>(std::sqrt(dx * dx + dy * dy));

                char biome = errorBiome;

                if (r >= innerRadius && r <= outerRadius) {
                    biome = ringBiome;
                }
                else {
                    int minDist = std::numeric_limits<int>::max();

                    for (const auto& center : centers) {
                        int cx = center.x - worldSize / 2;
                        int cy = center.y - worldSize / 2;
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
}

int WorldCreator::dist2(int x1, int y1, int x2, int y2) {
    return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
}

void WorldCreator::save_world_rle(const std::vector<std::vector<char>> &world) {
    if (!std::filesystem::exists("saves/")) {
        std::cerr << "Recreated saves folder";
        std::filesystem::create_directory("saves");
    }

    std::filesystem::create_directory("saves/" + worldName);
    std::string filename = "saves/" + worldName + "/world.dat";
    std::ofstream out(filename, std::ios::binary);

    if (!out.is_open()) {
        std::cerr << "Error: can't open world.dat\n";
        return;
    }

    int total_chunks = numberOfChunks * numberOfChunks;
    std::vector<ChunkHeader> headers(total_chunks);
    out.seekp(total_chunks * sizeof(ChunkHeader), std::ios::beg);

    for (int cy = 0; cy < numberOfChunks; ++cy) {
        for (int cx = 0; cx < numberOfChunks; ++cx) {
            int startX = cx * chunkSize;
            int startY = cy * chunkSize;

            std::streampos chunk_start = out.tellp();
            char current = world[startY][startX];
            unsigned char count = 1;

            for (int y = 0; y < chunkSize; ++y) {
                for (int x = 0; x < chunkSize; ++x) {
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
            headers[cy * numberOfChunks + cx] = { offset, size };
        }
    }

    out.seekp(0, std::ios::beg);
    for (const auto& header : headers) {
        out.write(reinterpret_cast<const char*>(&header), sizeof(ChunkHeader));
    }

    out.close();
}

std::string WorldCreator::getName() {
    std::vector<std::string> prefixes;
    std::vector<std::string> suffixes;

    if (prefixes.empty() || suffixes.empty()) {
        std::ifstream file("data/worldNames/names.txt");
        if (!file.is_open()) {
            std::cerr << "worldcreator:getName:can't open file names.txt\n";
        }

        std::string line;
        enum Section { NONE, PREFIX, SUFFIX } section = NONE;

        while (std::getline(file, line)) {
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
            if (line.empty()) continue;

            if (line[0] == '#') {
                if (line.find("PREFIX") != std::string::npos)
                    section = PREFIX;
                else if (line.find("SUFFIX") != std::string::npos)
                    section = SUFFIX;
                else
                    section = NONE;
                continue;
            }

            if (section == PREFIX)
                prefixes.push_back(line);
            else if (section == SUFFIX)
                suffixes.push_back(line);
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> pDist(0, static_cast<int>(prefixes.size() - 1));
    std::uniform_int_distribution<> sDist(0, static_cast<int>(suffixes.size() - 1));

    return prefixes[pDist(gen)] + " " + suffixes[sDist(gen)];
}
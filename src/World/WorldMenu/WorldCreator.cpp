#include "WorldCreator.h"
#include <iostream>
#include <utility>
#include <cmath>
#include <limits>
#include <fstream>
#include <random>
#include "Constants/WorldConst.h"
#include "Utilities/Logger/Logger.h"
#include "Constants/TilemapConst.h"

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

void WorldCreator::generate(int seed, std::string worldName)
{
    std::mt19937 rng(seed);
    this->worldName = std::move(worldName);

    std::uniform_int_distribution<int> dist(0, worldSize - 1);
    std::vector<BiomeCenter> centers;

    centers.push_back({ worldSize / 2, worldSize / 2, centerBiome });

    for (uint8_t b : innerBiomes) {
        int angle = dist(rng) % 360;
        int r = innerRadius * 2 / 3 + (dist(rng) % (innerRadius / 3));
        int cx = worldSize / 2 + static_cast<int>(r * std::cos(angle));
        int cy = worldSize / 2 + static_cast<int>(r * std::sin(angle));
        centers.push_back({ cx, cy, b });
    }

    for (uint8_t b : outerBiomes) {
        int angle = dist(rng) % 360;
        int r = outerRadius + 30 + (dist(rng) % 50);
        int cx = worldSize / 2 + static_cast<int>(r * std::cos(angle));
        int cy = worldSize / 2 + static_cast<int>(r * std::sin(angle));
        centers.push_back({ cx, cy, b });
    }

    for (int i = 0; i < extraCentersInner; ++i) {
        uint8_t b = innerBiomes[dist(rng) % innerBiomes.size()];
        int r = innerRadius / 2 + (dist(rng) % (innerRadius / 2));
        int angle = dist(rng) % 360;
        int cx = worldSize / 2 + static_cast<int>(r * std::cos(angle));
        int cy = worldSize / 2 + static_cast<int>(r * std::sin(angle));
        centers.push_back({ cx, cy, b });
    }

    for (int i = 0; i < extraCentersOuter; ++i) {
        uint8_t b = outerBiomes[dist(rng) % outerBiomes.size()];
        int r = outerRadius + 40 + (dist(rng) % 50);
        int angle = dist(rng) % 360;
        int cx = worldSize / 2 + static_cast<int>(r * std::cos(angle));
        int cy = worldSize / 2 + static_cast<int>(r * std::sin(angle));
        centers.push_back({ cx, cy, b });
    }

    std::vector<std::vector<uint8_t>> map(worldSize, std::vector<uint8_t>(worldSize, errorBiome));

    for (int y = 0; y < worldSize; ++y) {
        for (int x = 0; x < worldSize; ++x) {
            int dx = x - worldSize / 2;
            int dy = y - worldSize / 2;
            int r = static_cast<int>(std::sqrt(dx * dx + dy * dy));

            uint8_t biome = errorBiome;

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

    std::vector<Object> objectsList;
    std::vector<std::vector<bool>> occupiedTiles(worldSize, std::vector<bool>(worldSize, false));
    std::uniform_real_distribution<float> chance(0.0f, 1.0f);

    for (int y = 0; y < worldSize; ++y) {
        for (int x = 0; x < worldSize; ++x) {
            uint8_t biome = map[y][x];
            auto it = objectRules.find(biome);
            if (it != objectRules.end()) {
                for (const ObjectRule& rule : it->second) {
                    if (chance(rng) < rule.probability) {
                        ObjectType objType = static_cast<ObjectType>(rule.type);
                        auto propIt = objectPropertiesMap.find(objType);
                        if (propIt == objectPropertiesMap.end()) continue;

                        int w = static_cast<int>(propIt->second.size.x);
                        int h = static_cast<int>(propIt->second.size.y);

                        if (x + w > worldSize || y + h > worldSize)
                            continue;

                        bool canPlace = true;
                        for (int oy = 0; oy < h && canPlace; ++oy) {
                            for (int ox = 0; ox < w; ++ox) {
                                if (occupiedTiles[y + oy][x + ox]) {
                                    canPlace = false;
                                    break;
                                }
                            }
                        }
                        if (!canPlace) continue;

                        Object obj;
                        obj.type = objType;
                        obj.position = { (float)x * worldTileSize, (float)y * worldTileSize };
                        objectsList.push_back(obj);

                        for (int oy = 0; oy < h; ++oy) {
                            for (int ox = 0; ox < w; ++ox) {
                                occupiedTiles[y + oy][x + ox] = true;
                            }
                        }
                    }
                }
            }
        }
    }

    saveWorld(map, objectsList);
}

int WorldCreator::dist2(int x1, int y1, int x2, int y2) {
    return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
}

void WorldCreator::writeBiomeChunk(std::ofstream& out,
    const std::vector<std::vector<uint8_t>>& data,
    int startX, int startY) {
    uint8_t current = data[startY][startX];
    unsigned char count = 1;

    for (int y = 0; y < chunkSize; ++y) {
        for (int x = 0; x < chunkSize; ++x) {
            if (x == 0 && y == 0) continue;
            uint8_t next = data[startY + y][startX + x];
            if (next == current && count < 255) {
                count++;
            } else {
                out.write(reinterpret_cast<char*>(&count), 1);
                out.write(reinterpret_cast<char*>(&current), 1);
                current = next;
                count = 1;
            }
        }
    }

    out.write(reinterpret_cast<char*>(&count), 1);
    out.write(reinterpret_cast<char*>(&current), 1);
}

void WorldCreator::saveWorld(const std::vector<std::vector<uint8_t>>& biomes, 
                                  const std::vector<Object>& allObjects) {
    if (!std::filesystem::exists("saves/")) {
        mycerr << "Recreated saves folder";
        std::filesystem::create_directory("saves");
    }

    std::filesystem::create_directory("saves/" + worldName);
    std::string filename = "saves/" + worldName + "/world.dat";
    std::ofstream out(filename, std::ios::binary);

    if (!out.is_open()) {
        mycerr << "can't open world.dat";
        return;
    }

    int total_chunks = numberOfChunks * numberOfChunks;
    std::vector<ChunkHeader> headers(total_chunks);
    out.seekp(total_chunks * sizeof(ChunkHeader), std::ios::beg);

    for (int cy = 0; cy < numberOfChunks; ++cy) {
        for (int cx = 0; cx < numberOfChunks; ++cx) {
            int index = cy * numberOfChunks + cx;
            int startX = cx * chunkSize;
            int startY = cy * chunkSize;

            std::streampos biome_start = out.tellp();
            writeBiomeChunk(out, biomes, startX, startY);
            std::streampos biome_end = out.tellp();

            uint32_t biome_used = static_cast<uint32_t>(biome_end - biome_start);
            uint32_t biome_reserved = biome_used + reserveSizeBiome;
            out.write(std::string(biome_reserved - biome_used, '~').c_str(), biome_reserved - biome_used);

            std::streampos obj_start = out.tellp();
            std::vector<Object> chunkObjects = takeObjectsInchunk(allObjects, cx, cy, chunkSize, worldTileSize);
            writeObjectsChunk(out, chunkObjects, startX, startY, worldTileSize);
            std::streampos obj_end = out.tellp();

            uint32_t obj_used = static_cast<uint32_t>(obj_end - obj_start);
            uint32_t obj_reserved = obj_used + reserveSizeObject;
            out.write(std::string(obj_reserved - obj_used, '~').c_str(), obj_reserved - obj_used);
            
            headers[index] = {
                static_cast<uint32_t>(biome_start),
                biome_used,
                biome_reserved,

                static_cast<uint32_t>(obj_start),
                obj_used,
                obj_reserved
            };
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
            mycerr << "can't open file names.txt";
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

void WorldCreator::writeObjectsChunk(std::ofstream& out, 
    const std::vector<Object>& objects, int startTileX, int startTileY, int tileSize) {

    uint16_t count = static_cast<uint16_t>(objects.size());
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const Object& obj : objects) {
        FileObject fobj;
        fobj.type = static_cast<uint8_t>(obj.type);

        int tileX = static_cast<int>(obj.position.x / tileSize) - startTileX;
        int tileY = static_cast<int>(obj.position.y / tileSize) - startTileY;

        fobj.localX = static_cast<uint8_t>(tileX);
        fobj.localY = static_cast<uint8_t>(tileY);

        out.write(reinterpret_cast<const char*>(&fobj), sizeof(fobj));
    }
}

std::vector<Object> WorldCreator::takeObjectsInchunk(const std::vector<Object>& allObjects, int chunkX, int chunkY, int chunkSize, int tileSize) {
    std::vector<Object> result;

    int startX = chunkX * chunkSize * tileSize;
    int startY = chunkY * chunkSize * tileSize;
    int endX = startX + chunkSize * tileSize;
    int endY = startY + chunkSize * tileSize;

    for (const Object& obj : allObjects) {
        Rectangle r = obj.getBoundingBox(tileSize);
        if (r.x < endX && r.x + r.width > startX &&
            r.y < endY && r.y + r.height > startY) {
            result.push_back(obj);
        }
    }
    return result;
}
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
#include "Items/ItemBase/Item.h"
#include <algorithm>
#include "Utilities/World/Dist.h"

void WorldCreator::generate() 
{
    generate(generateWorldName());
}

void WorldCreator::generate(std::string worldName) 
{
    std::random_device rd;
    int seed = static_cast<int>(rd());
    generate(seed, std::move(worldName));
}

void WorldCreator::generate(int seed, std::string worldName)
{
    std::mt19937 rng(seed);
    this->worldName = std::move(worldName);

    // biomes
    std::vector<BiomeCenter> centers = { { worldSize / 2, worldSize / 2, centerBiome } };
    generateBiomeCenters(centers, innerBiomes, innerBiomes.size(), innerRadius * 2 / 3, innerRadius, rng);
    generateBiomeCenters(centers, outerBiomes, outerBiomes.size(), outerRadius + 30, outerRadius + 80, rng);
    generateBiomeCenters(centers, innerBiomes, extraCentersInner, innerRadius / 2, innerRadius, rng);
    generateBiomeCenters(centers, outerBiomes, extraCentersOuter, outerRadius + 40, outerRadius + 90, rng);

    std::vector<std::vector<uint8_t>> biomeList(worldSize, std::vector<uint8_t>(worldSize, errorBiome));

    for (int y = 0; y < worldSize; ++y) 
    {
        for (int x = 0; x < worldSize; ++x) 
        {
            int dx = x - worldSize / 2;
            int dy = y - worldSize / 2;
            int r = static_cast<int>(std::sqrt(dx * dx + dy * dy));

            uint8_t biome = errorBiome;

            if (r >= innerRadius && r <= outerRadius) { biome = ringBiome; } 
            else 
            {
                int minDist = std::numeric_limits<int>::max();
                for (const auto& center : centers) 
                {
                    int cr = static_cast<int>(std::hypot(center.x - worldSize / 2, center.y - worldSize / 2));
                    bool sameZone = (r < innerRadius && cr < innerRadius) || (r > outerRadius && cr > outerRadius) || (r <= innerRadius && center.type == 'F');

                    int d2 = sameZone ? dist2(x, y, center.x, center.y) : std::numeric_limits<int>::max();
                    if (d2 < minDist) 
                    {
                        minDist = d2;
                        biome = center.type;
                    }
                }
            }

            biomeList[y][x] = biome;
        }
    }

    // objects
    auto objectsList = generateObjects(biomeList, rng);

    // create world
    createWorld(biomeList, objectsList);
    createPlayerDir();
}

void WorldCreator::generateBiomeCenters(std::vector<BiomeCenter>& centers, const std::vector<uint8_t>& biomes, 
                                        int count, int minR, int maxR, std::mt19937& rng)
{
    std::uniform_int_distribution<int> angleDist(0, 359);
    std::uniform_int_distribution<int> radiusDist(minR, maxR);

    for (int i = 0; i < count; ++i) 
    {
        uint8_t b = biomes[rng() % biomes.size()];
        int angle = angleDist(rng);
        int r = radiusDist(rng);
        int cx = worldSize / 2 + static_cast<int>(r * std::cos(angle * M_PI / 180.0));
        int cy = worldSize / 2 + static_cast<int>(r * std::sin(angle * M_PI / 180.0));
        centers.push_back({ cx, cy, b });
    }
}

std::vector<Object> WorldCreator::generateObjects(const std::vector<std::vector<uint8_t>>& map, std::mt19937& rng)
{
    std::vector<Object> objectsList;
    std::vector<std::vector<bool>> occupied(worldSize, std::vector<bool>(worldSize, false));
    std::uniform_real_distribution<float> chance(0.0f, 1.0f);

    for (int y = 0; y < worldSize; ++y) 
    {
        for (int x = 0; x < worldSize; ++x) 
        {
            uint8_t biome = map[y][x];
            auto ruleIt = objectRules.find(biome);
            if (ruleIt == objectRules.end()) { continue; }

            for (const ObjectRule& rule : ruleIt->second) 
            {
                if (chance(rng) >= rule.probability) { continue; }

                ObjectType type = static_cast<ObjectType>(rule.type);
                auto propIt = objectPropertiesMap.find(type);
                if (propIt == objectPropertiesMap.end()) { continue; }

                int w = static_cast<int>(propIt->second.size.x);
                int h = static_cast<int>(propIt->second.size.y);

                if (x + w > worldSize || y + h > worldSize) { continue; }

                bool fits = true;
                for (int oy = 0; oy < h && fits; ++oy) 
                {
                    for (int ox = 0; ox < w; ++ox) 
                    {
                        if (occupied[y + oy][x + ox]) { fits = false; }
                    }
                }

                if (!fits) { continue; }

                Object obj;
                obj.type = type;
                obj.position = { (float) x * worldTileSize, (float) y * worldTileSize };
                objectsList.push_back(obj);

                for (int oy = 0; oy < h; ++oy) 
                {
                    for (int ox = 0; ox < w; ++ox) { occupied[y + oy][x + ox] = true; }
                }
            }
        }
    }

    return objectsList;
}

std::string WorldCreator::generateWorldName() // generate Name if world name is empty
{
    std::vector<std::string> prefixes;
    std::vector<std::string> suffixes;

    if (prefixes.empty() || suffixes.empty()) 
    {
        std::ifstream file("data/worldNames/names.txt");
        if (!file.is_open()) { mycerr << "can't open file names.txt"; }

        std::string line;
        enum Section { NONE, PREFIX, SUFFIX } section = NONE;

        while (std::getline(file, line)) 
        {
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
            if (line.empty()) { continue; }

            if (line[0] == '#') 
            {
                if      (line.find("PREFIX") != std::string::npos) { section = PREFIX; }
                else if (line.find("SUFFIX") != std::string::npos) { section = SUFFIX; }
                else { section = NONE; }
                continue;
            }

            if (section == PREFIX) { prefixes.push_back(line); }
            else if (section == SUFFIX) { suffixes.push_back(line); }
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> pDist(0, static_cast<int>(prefixes.size() - 1));
    std::uniform_int_distribution<> sDist(0, static_cast<int>(suffixes.size() - 1));

    return prefixes[pDist(gen)] + " " + suffixes[sDist(gen)];
}

void WorldCreator::createWorld(const std::vector<std::vector<uint8_t>>& biomes, const std::vector<Object>& objects) 
{
    // check for files
    if (!std::filesystem::exists("saves/")) 
    {
        mycerr << "Recreated saves and worlds folder";
        std::filesystem::create_directory("saves");
        std::filesystem::create_directory("saves/worlds");
    }

    else if (!std::filesystem::exists("saves/worlds")) 
    {
        mycerr << "Recreated saves/worlds folder";
        std::filesystem::create_directory("saves/worlds");
    }

    std::filesystem::create_directory("saves/worlds/" + worldName);
    std::string filename = "saves/worlds/" + worldName + "/world.dat";
    std::ofstream out(filename, std::ios::binary);

    if (!out.is_open()) 
    {
        mycerr << "can't open world.dat";
        return;
    }

    // saving
    int total_chunks = numberOfChunks * numberOfChunks;
    std::vector<ChunkHeader> headers(total_chunks);
    out.seekp(total_chunks * sizeof(ChunkHeader), std::ios::beg);

    for (int cy = 0; cy < numberOfChunks; ++cy) 
    {
        for (int cx = 0; cx < numberOfChunks; ++cx) 
        {
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
            std::vector<Object> chunkObjects = takeObjectsInchunk(objects, cx, cy, chunkSize, worldTileSize);
            writeObjectsChunk(out, chunkObjects, startX, startY, worldTileSize);
            std::streampos obj_end = out.tellp();

            uint32_t obj_used = static_cast<uint32_t>(obj_end - obj_start);
            uint32_t obj_reserved = obj_used + reserveSizeObject;
            out.write(std::string(obj_reserved - obj_used, '~').c_str(), obj_reserved - obj_used);
            
            headers[index] = {static_cast<uint32_t>(biome_start), biome_used, biome_reserved,
                              static_cast<uint32_t>(obj_start), obj_used, obj_reserved };
        }
    }

    out.seekp(0, std::ios::beg);
    for (const auto& header : headers) { out.write(reinterpret_cast<const char*>(&header), sizeof(ChunkHeader)); }

    out.close();
}

void WorldCreator::writeBiomeChunk(std::ofstream& out, const std::vector<std::vector<uint8_t>>& data, int startX, int startY) 
{
    uint8_t current = data[startY][startX];
    unsigned char count = 1;

    for (int y = 0; y < chunkSize; ++y) 
    {
        for (int x = 0; x < chunkSize; ++x) 
        {
            if (x == 0 && y == 0) { continue; }
            uint8_t next = data[startY + y][startX + x];

            if (next == current && count < 255) { count++; } 
            else 
            {
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

void WorldCreator::writeObjectsChunk(std::ofstream& out, const std::vector<Object>& objects, int startTileX, int startTileY, int tileSize) 
{
    uint16_t count = static_cast<uint16_t>(objects.size());
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const Object& obj : objects) 
    {
        FileObject fobj;
        fobj.type = static_cast<uint8_t>(obj.type);

        int tileX = static_cast<int>(obj.position.x / tileSize) - startTileX;
        int tileY = static_cast<int>(obj.position.y / tileSize) - startTileY;

        fobj.localX = static_cast<uint8_t>(tileX);
        fobj.localY = static_cast<uint8_t>(tileY);

        out.write(reinterpret_cast<const char*>(&fobj), sizeof(fobj));
    }
}

std::vector<Object> WorldCreator::takeObjectsInchunk(const std::vector<Object>& objects, int chunkX, int chunkY, int chunkSize, int tileSize) 
{
    std::vector<Object> result;

    int startX = chunkX * chunkSize * tileSize;
    int startY = chunkY * chunkSize * tileSize;
    int endX = startX + chunkSize * tileSize;
    int endY = startY + chunkSize * tileSize;

    for (const Object& obj : objects) 
    {
        Rectangle r = obj.getBoundingBox(tileSize);
        if (r.x < endX && r.x + r.width > startX && r.y < endY && r.y + r.height > startY) { result.push_back(obj); }
    }
    return result;
}

void WorldCreator::createPlayerDir() 
{
    std::string playerPath = "saves/worlds/" + worldName + "/playerData";
    std::filesystem::create_directories(playerPath);

    std::string inventoryPath = playerPath + "/inventory.inv";
    std::ofstream out(inventoryPath, std::ios::binary);

    if (!out) 
    {
        mycerr << "Can't create player dir in " << inventoryPath;
        return;
    }
    
    for (int i = 0; i < slotCount; ++i) // inventory
    {
        uint16_t id = static_cast<uint16_t>(ItemID::NONE);
        uint8_t count = 0;
        uint16_t durability = 0;

        out.write(reinterpret_cast<const char*>(&id), sizeof(id));
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    }
}
#pragma once
#include <vector>
#include <cstdlib>
#include <string>
#include <filesystem>
#include "Const.h"
#include "World/ChunkSystem/Object.h"
#include <random>

// TODO configure world size

class WorldCreator {
private:
    std::string worldName;

public:
    WorldCreator() = default;
    ~WorldCreator() = default;

    void generate(); // random name and seed
    void generate(std::string worldName); // random seed
    void generate(int seed, std::string worldName); // not random

private:
    // generation
    void generateBiomeCenters(std::vector<BiomeCenter>& centers, const std::vector<uint8_t>& biomes, int count, int minR, int maxR, std::mt19937& rng);
    std::vector<Object> generateObjects(const std::vector<std::vector<uint8_t>>& map, std::mt19937& rng);
    std::string generateWorldName();
    
    // saving
    void createWorld(const std::vector<std::vector<uint8_t>>& world, const std::vector<Object>& allObjects);
    void writeBiomeChunk(std::ofstream& out,const std::vector<std::vector<uint8_t>>& data,int startX, int startY);
    void writeObjectsChunk(std::ofstream& out, const std::vector<Object>& objects, int startTileX, int startTileY, int tileSize);
    std::vector<Object> takeObjectsInchunk(const std::vector<Object>& allObjects, int chunkX, int chunkY, int chunkSize, int tileSize);
    void createPlayerDir();
};
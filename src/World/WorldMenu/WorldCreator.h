#ifndef WORLDCREATOR_H
#define WORLDCREATOR_H

#include <vector>
#include <cstdlib>
#include <string>
#include <filesystem>
#include "World/ChunkSystem/Biome.h"
#include "World/ChunkSystem/Object.h"

class WorldCreator {
private:
    static constexpr int extraCentersInner = 4;
    static constexpr int extraCentersOuter = 10;
    static constexpr int innerRadius = 200;
    static constexpr int ringWidth = 30;
    static constexpr int outerRadius = innerRadius + ringWidth;

    const uint8_t errorBiome = BIOME_UNKNOWN;
    const uint8_t centerBiome = BIOME_FOREST;
    const std::vector<uint8_t> innerBiomes = {BIOME_WINTER, BIOME_SAND, BIOME_SWAMP, BIOME_VOLCANO};
    const std::vector<uint8_t> outerBiomes = {BIOME_GLASS, BIOME_BEACH, BIOME_CRYSTAL, BIOME_MOUNTAIN};
    const uint8_t ringBiome = BIOME_EXIT;

// TODO configure world size

    std::string worldName;

    struct BiomeCenter {
        int x, y;
        uint8_t type;
    };

public:
    WorldCreator();
    ~WorldCreator();

    void generate();
    void generate(std::string worldName);
    void generate(int seed, std::string worldName);

private:
    static int dist2(int x1, int y1, int x2, int y2);

    std::string getName();

    void writeBiomeChunk(std::ofstream& out,const std::vector<std::vector<uint8_t>>& data,int startX, int startY);
    void saveWorld(const std::vector<std::vector<uint8_t>>& world, const std::vector<Object>& allObjects);
    void writeObjectsChunk(std::ofstream& out, const std::vector<Object>& objects, int startTileX, int startTileY, int tileSize);
    std::vector<Object> takeObjectsInchunk(const std::vector<Object>& allObjects, int chunkX, int chunkY, int chunkSize, int tileSize);
};

#endif //WORLDCREATOR_H

#ifndef WORLDCONST_H
#define WORLDCONST_H

#include <cstdint>

constexpr int worldSize = 1024;
const int chunkSize = 32;
constexpr int numberOfChunks = worldSize / chunkSize;

const int tileSize = 64;
const int sourceTileSize = 16;
const int tilesPerRow = 10; // connected with sourceTileSizes tilemap.width = 160 / 16 = 10

const int renderDistance = 6;
const int unloadMargin = 0;

const int handDistance = 4;

struct ChunkHeader {
    uint32_t offsetBiome;
    uint32_t sizeBiome;
    uint32_t offsetObject;
    uint32_t sizeObject;
};

// todo find perfect chunk/world/tile size

#endif //WORLDCONST_H

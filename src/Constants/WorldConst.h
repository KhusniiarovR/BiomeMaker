#ifndef WORLDCONST_H
#define WORLDCONST_H

#include <cstdint>

constexpr int worldSize = 1024;
const int chunkSize = 32;
constexpr int numberOfChunks = worldSize / chunkSize;
const int tileSize = 16;
const int chunkPixelSize = chunkSize * tileSize;
const int sourceTileSize = 16;
const int tilesPerRow = 10; // connected with sourceTileSizes tilemap.width = 160 / 16 = 10
const float padding = 0.01f;

const int renderDistance = 2;
const int unloadMargin = 0;

const int handDistance = 4;

const int reserveSizeBiome = 40;
const int reserveSizeObject = 80;

struct ChunkHeader {
    uint32_t offsetBiome;
    uint32_t dataSizeBiome;
    uint32_t reservedSizeBiome;

    uint32_t offsetObject;
    uint32_t dataSizeObject;
    uint32_t reservedSizeObject;
};

// TODO find perfect chunk/world/tile size

#endif //WORLDCONST_H

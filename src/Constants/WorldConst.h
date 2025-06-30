#ifndef WORLDCONST_H
#define WORLDCONST_H

#include <cstdint>

// world 
constexpr int worldSize = 1024;
const int chunkSize = 32;
constexpr int numberOfChunks = worldSize / chunkSize;
const int chunkPixelSize = chunkSize * 16; // 16 = worldTileSize

// world loading/unloading
const int renderDistance = 2;
const int unloadMargin = 0;

// player
const int handDistance = 4;


// world saving
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

#endif //WORLDCONST_H

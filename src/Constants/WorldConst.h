#pragma once
#include <cstdint>

// world 
constexpr int worldSize = 1024;                       // world size in tiles
const int chunkSize = 32;                             // size of one chunk
constexpr int numberOfChunks = worldSize / chunkSize; // number of chunks in the world
const int chunkPixelSize = chunkSize * 16;            // 16 = worldTileSize from tilemapConst.h

// world loading/unloading
const int renderDistance = 2; // how far from player chunks will be loaded
const int unloadMargin = 0;   // when chunks will be unloaded

// world saving
const int reserveSizeBiome = 40;  // reserved size for biome to not save world every time again
const int reserveSizeObject = 80; // reserved size for biome to not save world every time again

struct ChunkHeader {
    uint32_t offsetBiome;       // where is biome
    uint32_t dataSizeBiome;     // how much biome data takes
    uint32_t reservedSizeBiome; // how much free space left

    uint32_t offsetObject;      // where is biome
    uint32_t dataSizeObject;    // how much objects data takes
    uint32_t reservedSizeObject;// how much free space left
};
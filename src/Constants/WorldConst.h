#ifndef WORLDCONST_H
#define WORLDCONST_H

#include <cstdint>

constexpr int worldSize = 1024;
const int chunkSize = 32;
constexpr int numberOfChunks = worldSize / chunkSize;

const int tileSize = 16;

const int renderDistance = 6;
const int unloadMargin = 0;

struct ChunkHeader {
    uint32_t offset;
    uint32_t size;
};

// todo find perfect chunk/world/tile size

#endif //WORLDCONST_H

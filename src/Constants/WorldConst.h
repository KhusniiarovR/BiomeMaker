#ifndef WORLDCONST_H
#define WORLDCONST_H

#include <cstdint>

constexpr int WORLD_SIZE = 1024;
const int CHUNK_SIZE = 32;
constexpr int NUM_CHUNKS = WORLD_SIZE / CHUNK_SIZE;

const int TILE_SIZE = 16;

const int renderDistance = 6;
const int unloadMargin = 0;

struct ChunkHeader {
    uint32_t offset;
    uint32_t size;
};


// todo find perfect chunk/world/tile size

#endif //WORLDCONST_H

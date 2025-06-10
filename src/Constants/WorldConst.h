#ifndef WORLDCONST_H
#define WORLDCONST_H

constexpr int WORLD_SIZE = 1024;
const int TILE_SIZE = 16;
const int CHUNK_SIZE = 32;
constexpr int NUM_CHUNKS = WORLD_SIZE / CHUNK_SIZE;

const int RENDER_DISTANCE = 6;
const int unloadMargin = 0;

const char BIOME_SYMBOLS[] = { 'F', 'O', 'W', 'S', 'V', 'G', 'B', 'C', 'M', 'E' };

// todo find perfect chunk/world/tile size

#endif //WORLDCONST_H

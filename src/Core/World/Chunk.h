#ifndef CHUNK_H
#define CHUNK_H

#include "Constants/WorldConst.h"
#include "Biome.h"
#include "Utilities/World/PerlinNoise.h"

class Chunk {
public:
    int x, y;
    const Biome* tiles[CHUNK_SIZE][CHUNK_SIZE];

    Chunk(int cx, int cy) : x(cx), y(cy) {
        Generate();
    }

    void Generate() {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                float worldX = (this->x * CHUNK_SIZE + x) * TILE_SIZE;
                float worldY = (this->y * CHUNK_SIZE + y) * TILE_SIZE;

                float noise = PerlinNoise(worldX, worldY);

                if (noise < 0.4f)
                    tiles[y][x] = &BIOMES[0];
                else if (noise < 0.7f)
                    tiles[y][x] = &BIOMES[1];
                else
                    tiles[y][x] = &BIOMES[2];
            }
        }
    }

    void Draw() const {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                float worldX = (this->x * CHUNK_SIZE + x) * TILE_SIZE;
                float worldY = (this->y * CHUNK_SIZE + y) * TILE_SIZE;
                DrawRectangle(worldX, worldY, TILE_SIZE, TILE_SIZE, tiles[y][x]->color);
            }
        }
    }
};

#endif //CHUNK_H

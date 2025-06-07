#ifndef CHUNK_H
#define CHUNK_H

#include "Constants/WorldConst.h"
#include "Biome.h"
#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>

class Chunk {
public:
    int x, y;
    const Biome* tiles[CHUNK_SIZE][CHUNK_SIZE];

    Chunk(int cx, int cy) : x(cx), y(cy) {
        Generate();
    }

    // todo add world creating menu
    
    // todo saves directory for worlds
    void Generate() {
        std::string filename = "world/chunk_" + std::to_string(x) + "_" + std::to_string(y) + ".chunk";
        std::ifstream in(filename, std::ios::binary);

        if (!in.is_open()) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int x = 0; x < CHUNK_SIZE; x++) {
                    tiles[y][x] = &BIOMES[0];
                }
            }
            return;
        }

        int tx = 0, ty = 0;

        while (!in.eof()) {
            unsigned char count;
            char symbol;

            in.read(reinterpret_cast<char*>(&count), 1);
            in.read(&symbol, 1);

            if (in.eof()) break;

            const Biome* biomePtr = SymbolToBiome(symbol);

            for (int i = 0; i < count; ++i) {
                if (ty >= CHUNK_SIZE) break;

                tiles[ty][tx] = biomePtr;
                tx++;

                if (tx >= CHUNK_SIZE) {
                    tx = 0;
                    ty++;
                }
            }
        }
        in.close();
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

private:
    const Biome* SymbolToBiome(char symbol) {
        for (int i = 0; i < sizeof(BIOME_SYMBOLS); ++i) {
            if (BIOME_SYMBOLS[i] == symbol)
                return &BIOMES[i+1]; // first is none biome
        }
        std::cerr << "Chunk: unknown symbol " << symbol << "\n";
        return &BIOMES[0];
    }
};

#endif //CHUNK_H

#ifndef CHUNK_H
#define CHUNK_H

#include "Constants/WorldConst.h"
#include "Biome.h"
#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>

struct ChunkHeader {
    uint32_t offset;
    uint32_t size;
};

class Chunk {
public:
    int x, y;
    const Biome* tiles[CHUNK_SIZE][CHUNK_SIZE];

    Chunk(int cx, int cy, const std::vector<ChunkHeader>& headers, std::ifstream& file) : x(cx), y(cy) {
        Generate(headers, file);
    }

    // todo add world creating menu
    // todo saves directory for worlds

    void Generate(const std::vector<ChunkHeader>& headers, std::ifstream& file) {
        if (x < 0 || x >= NUM_CHUNKS || y < 0 || y >= NUM_CHUNKS) {
            for (int ty = 0; ty < CHUNK_SIZE; ++ty)
                for (int tx = 0; tx < CHUNK_SIZE; ++tx)
                    tiles[ty][tx] = &BIOMES[0];
            return;
        }
        int index = y * NUM_CHUNKS + x;

        const ChunkHeader& header = headers[index];

        file.seekg(header.offset, std::ios::beg);
        int tx = 0, ty = 0;

        while (file.tellg() < static_cast<std::streampos>(header.offset + header.size)) {
            unsigned char count;
            char symbol;
            file.read(reinterpret_cast<char*>(&count), 1);
            file.read(&symbol, 1);

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

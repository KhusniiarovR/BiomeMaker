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
    const Biome* tiles[chunkSize][chunkSize];

    Chunk(int cx, int cy, const std::vector<ChunkHeader>& headers, std::ifstream& file) : x(cx), y(cy) {
        Generate(headers, file);
    }

    // todo add world creating menu
    // todo saves directory for worlds

    void Generate(const std::vector<ChunkHeader>& headers, std::ifstream& file) {
        if (x < 0 || x >= numberOfChunks || y < 0 || y >= numberOfChunks) {
            for (int ty = 0; ty < chunkSize; ++ty)
                for (int tx = 0; tx < chunkSize; ++tx)
                    tiles[ty][tx] = &BIOMES[0];
            return;
        }
        int index = y * numberOfChunks + x;

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
                if (ty >= chunkSize) break;

                tiles[ty][tx] = biomePtr;
                tx++;
                if (tx >= chunkSize) {
                    tx = 0;
                    ty++;
                }
            }
        }
    }

    void Draw(Texture2D& tilemap) const {
        for (int y = 0; y < chunkSize; y++) {
            for (int x = 0; x < chunkSize; x++) {
                float worldX = (this->x * chunkSize + x) * tileSize;
                float worldY = (this->y * chunkSize + y) * tileSize;

                int tileIndex = 1;
                int tileX = (tileIndex % tilesPerRow) * sourceTileSize;
                int tileY = (tileIndex / tilesPerRow) * sourceTileSize;

                Rectangle sourceRec = { (float)tileX, (float)tileY, (float)sourceTileSize, (float)sourceTileSize };
                Rectangle destRec = { worldX, worldY, (float)tileSize, (float)tileSize };

                DrawTexturePro(tilemap, sourceRec, destRec, {0, 0}, 0.0f, WHITE);
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

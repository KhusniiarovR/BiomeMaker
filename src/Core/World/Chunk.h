#ifndef CHUNK_H
#define CHUNK_H

#include "Constants/WorldConst.h"
#include "Biome.h"
#include "Object.h"
#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>

class Chunk {
public:
    int x, y;
    std::vector<std::vector<uint8_t>> tiles{chunkSize, std::vector<uint8_t>(chunkSize)};
    std::vector<std::vector<Object>> objectTiles{chunkSize, std::vector<Object>(chunkSize)};

    Chunk(int cx, int cy, const std::vector<ChunkHeader>& headers, std::ifstream& file) : x(cx), y(cy) {
        Generate(headers, file);
    }

    // todo add world creating menu

    void Generate(const std::vector<ChunkHeader>& headers, std::ifstream& file) {
        if (x < 0 || x >= numberOfChunks || y < 0 || y >= numberOfChunks) {
            for (int ty = 0; ty < chunkSize; ++ty)
                for (int tx = 0; tx < chunkSize; ++tx)
                    tiles[ty][tx] = 0;
            return;
        }

        int index = y * numberOfChunks + x;
        const ChunkHeader& header = headers[index];

        file.seekg(header.offsetBiome, std::ios::beg);
        int tx = 0, ty = 0;

        while (file.tellg() < static_cast<std::streampos>(header.offsetBiome + header.sizeBiome)) {
            unsigned char count;
            char symbol;
            file.read(reinterpret_cast<char*>(&count), 1);
            file.read(&symbol, 1);

            const Biome* biome = SymbolToBiome(symbol);

            for (int i = 0; i < count; ++i) {
                if (ty >= chunkSize) break;

                int worldX = this->x * chunkSize + tx;
                int worldY = this->y * chunkSize + ty;
                uint32_t hash = worldX * 73856093u ^ worldY * 19349663u;

                tiles[ty][tx] = ChooseTileIndex(biome, hash);

                tx++;
                if (tx >= chunkSize) {
                    tx = 0;
                    ty++;
                }
            }
        }
        file.seekg(header.offsetObject, std::ios::beg);
        {
            int tx = 0, ty = 0;
            while (file.tellg() < static_cast<std::streampos>(header.offsetObject + header.sizeObject)) {
                unsigned char count;
                char symbol;
                file.read(reinterpret_cast<char*>(&count), 1);
                file.read(&symbol, 1);

                for (int i = 0; i < count; ++i) {
                    if (ty >= chunkSize) break;

                    objectTiles[ty][tx] = symbolToObject(symbol);

                    tx++;
                    if (tx >= chunkSize) {
                        tx = 0;
                        ty++;
                    }
                }
            }
        }
    }

    void Draw(Texture2D& tilemap) const {
        const int chunkX = this->x * chunkSize;
        const int chunkY = this->y * chunkSize;
        for (int y = 0; y < chunkSize; y++) {
            for (int x = 0; x < chunkSize; x++) {
                float worldX = (chunkX + x) * tileSize;
                float worldY = (chunkY + y) * tileSize;

                int tileIndex = tiles[y][x];
                int tileX = (tileIndex % tilesPerRow) * sourceTileSize;
                int tileY = (tileIndex / tilesPerRow) * sourceTileSize;

                Rectangle sourceRec = { (float)tileX, (float)tileY, (float)sourceTileSize, (float)sourceTileSize };
                Rectangle destRec = { worldX, worldY, (float)tileSize, (float)tileSize };

                DrawTexturePro(tilemap, sourceRec, destRec, {0, 0}, 0.0f, WHITE);

                const Object& obj = objectTiles[y][x];
                if (obj.type != ObjectType::None) {
                    int objectTileIndex = objectTypeToTile(obj.type);
                    int objTileX = (objectTileIndex % tilesPerRow) * sourceTileSize;
                    int objTileY = (objectTileIndex / tilesPerRow) * sourceTileSize;

                    Rectangle objSource = { (float)objTileX, (float)objTileY, (float)sourceTileSize, (float)sourceTileSize };
                    DrawTexturePro(tilemap, objSource, destRec, {0, 0}, 0.0f, WHITE);
                }
            }
        }
    }

//todo save modified chunk 

private:
    const Biome* SymbolToBiome(char symbol) {
        for (int i = 0; i < sizeof(BIOME_SYMBOLS); ++i) {
            if (BIOME_SYMBOLS[i] == symbol)
                return &BIOMES[i+1]; // first is none biome
        }
        std::cerr << "Chunk: unknown symbol " << symbol << "\n";
        return &BIOMES[0];
    }

    uint8_t ChooseTileIndex(const Biome* biome, uint32_t seed) {
        float roll = (seed % 10000) / 10000.0f;
        float cumulative = 0.0f;
        
        for (const auto& [index, chance] : biome->tileVariants) {
            cumulative += chance;
            if (roll < cumulative)
                return index;
        }

        return biome->tileVariants.front().first;
    }

    int objectTypeToTile(ObjectType objectType) const {
        auto it = objectTileMap.find(objectType);
        return (it != objectTileMap.end()) ? it->second : 0;
    }

    Object symbolToObject(char symbol) {
    switch (symbol) {
        case 'T': return { ObjectType::Tree };
        case 'R': return { ObjectType::Rock };
        case 'B': return { ObjectType::Bush };
        case ' ': return { ObjectType::None };
        default: {
            std::cerr << "Chunk::unknown object" << symbol << "\n";
            return {};
        };
    }
}
};

#endif //CHUNK_H

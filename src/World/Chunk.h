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
    std::vector<std::vector<char>> tiles{chunkSize, std::vector<char>(chunkSize)};
    std::vector<std::vector<Object>> objectTiles{chunkSize, std::vector<Object>(chunkSize)};
    bool isModified = false;

    Chunk(int cx, int cy, const std::vector<ChunkHeader>& headers, std::ifstream& file);
    void Generate(const std::vector<ChunkHeader>& headers, std::ifstream& file);
    void Draw(Texture2D& tilemap) const;

private:
    const Biome* SymbolToBiome(char symbol);
    Object symbolToObject(char symbol);

    uint8_t ChooseTileIndex(const Biome* biome, uint32_t seed);
    int objectTypeToTile(ObjectType objectType) const;
};

#endif //CHUNK_H

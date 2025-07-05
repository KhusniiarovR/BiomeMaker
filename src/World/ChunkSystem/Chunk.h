#ifndef CHUNK_H
#define CHUNK_H

#include "Biome.h"
#include "Object.h"
#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>
#include "raylib.h"
#include "Constants/WorldConst.h"

class Chunk {
public:
    int x, y;
    std::vector<std::vector<uint8_t>> tiles{chunkSize, std::vector<uint8_t>(chunkSize)};
    std::vector<Object> objects;
    bool isModified = false;

    Chunk(int cx, int cy, const std::vector<ChunkHeader>& headers, std::ifstream& file);
    void Generate(const std::vector<ChunkHeader>& headers, std::ifstream& file);
    void DrawTiles(Texture2D& tilemap) const;
    void DrawObjects(Texture2D& tilemap) const;

private:
    uint8_t ChooseTileIndex(const Biome* biome, uint32_t seed);
    int objectTypeToTile(ObjectType objectType) const;
};

#endif //CHUNK_H

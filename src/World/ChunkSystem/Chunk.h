#pragma once
#include "Biome.h"
#include "Object.h"
#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>
#include "raylib.h"
#include "Constants/WorldConst.h"

class Chunk {
private:
    int x, y;
    bool isModified = false;
    std::vector<std::vector<uint8_t>> tiles{chunkSize, std::vector<uint8_t>(chunkSize)};
    std::vector<Object> objects;

public:
    Chunk(int cx, int cy, const std::vector<ChunkHeader>& headers, std::ifstream& file);
    void generate(const std::vector<ChunkHeader>& headers, std::ifstream& file);
    void renderTiles(Texture2D& tilemap) const;
    void renderObjects(Texture2D& tilemap) const;

    // getters and setters
    bool getModified() const { return isModified; }
    void setModified(bool mod) { isModified = mod; }
    const std::vector<std::vector<uint8_t>>& getTiles() const { return tiles; }
    std::vector<std::vector<uint8_t>>& getTiles() { return tiles; }
    const std::vector<Object>& getObjects() const { return objects; }
    std::vector<Object>& getObjects() { return objects; }

private:
    uint8_t chooseTileIndex(const Biome* biome, uint32_t seed);
    int objectTypeToTile(ObjectType objectType) const;
    int getTileRotation(int globalX, int globalY) const;
};
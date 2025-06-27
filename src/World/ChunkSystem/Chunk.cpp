#include "Chunk.h"
#include "Utilities/Logger/Logger.h"

Chunk::Chunk(int cx, int cy, const std::vector<ChunkHeader>& headers, std::ifstream& file) : x(cx), y(cy) {
    Generate(headers, file);
}

void Chunk::Generate(const std::vector<ChunkHeader>& headers, std::ifstream& file)  {
    if (x < 0 || x >= numberOfChunks || y < 0 || y >= numberOfChunks) {
        for (int ty = 0; ty < chunkSize; ++ty)
            for (int tx = 0; tx < chunkSize; ++tx)
                tiles[ty][tx] = 0;
        return;
    }

    int index = y * numberOfChunks + x;
    const ChunkHeader& header = headers[index];

    file.seekg(header.offsetBiome, std::ios::beg);
    {
        int tx = 0, ty = 0;
        std::streampos biome_end = header.offsetBiome + header.dataSizeBiome;

        while (file.tellg() < biome_end) {
            unsigned char count;
            uint8_t biomeId;

            file.read(reinterpret_cast<char*>(&count), 1);
            file.read(reinterpret_cast<char*>(&biomeId), 1);

            const Biome* biome = (biomeId < BIOMES.size()) ? &BIOMES[biomeId] : &BIOMES[0];

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
    }

    file.seekg(header.offsetObject, std::ios::beg);
    {
        int tx = 0, ty = 0;
        std::streampos object_end = header.offsetObject + header.dataSizeObject;

        while (file.tellg() < object_end) {
            unsigned char count;
            uint8_t id;

            file.read(reinterpret_cast<char*>(&count), 1);
            file.read(reinterpret_cast<char*>(&id), 1);

            for (int i = 0; i < count; ++i) {
                if (ty >= chunkSize) break;

                objectTiles[ty][tx] = Object{ static_cast<ObjectType>(id) };

                tx++;
                if (tx >= chunkSize) {
                    tx = 0;
                    ty++;
                }
            }
        }
    }
}

void Chunk::Draw(Texture2D& tilemap) const  {
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

uint8_t Chunk::ChooseTileIndex(const Biome* biome, uint32_t seed) {
    float roll = (seed % 10000) / 10000.0f;
    float cumulative = 0.0f;
    
    for (const auto& [index, chance] : biome->tileVariants) {
        cumulative += chance;
        if (roll < cumulative)
            return index;
    }

    return biome->tileVariants.front().first;
}

int Chunk::objectTypeToTile(ObjectType objectType) const {
    auto it = objectTileMap.find(objectType);
    return (it != objectTileMap.end()) ? it->second : 0;
}
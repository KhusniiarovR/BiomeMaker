#include "Chunk.h"
#include "Utilities/Logger/Logger.h"
#include "Constants/WorldConst.h"
#include "Constants/TilemapConst.h"
#include "Object.h"

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
        uint16_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(count));

        objects.clear();
        objects.reserve(count);

        for (uint16_t i = 0; i < count; ++i) {
            FileObject fobj;
            file.read(reinterpret_cast<char*>(&fobj), sizeof(fobj));

            Object obj;
            obj.type = static_cast<ObjectType>(fobj.type);

            int startTileX = x * chunkSize;
            int startTileY = y * chunkSize;

            obj.position.x = (startTileX + fobj.localX) * worldTileSize;
            obj.position.y = (startTileY + fobj.localY) * worldTileSize;

            objects.push_back(obj);
        }
    }
}

void Chunk::DrawTiles(Texture2D& tilemap) const {
    const int chunkX = this->x * chunkSize;
    const int chunkY = this->y * chunkSize;

    for (int y = 0; y < chunkSize; y++) {
        for (int x = 0; x < chunkSize; x++) {
            int worldX = (chunkX + x) * worldTileSize;
            int worldY = (chunkY + y) * worldTileSize;

            int tileIndex = tiles[y][x];
            int tileX = (tileIndex % worldTilesPerRow) * worldSourceTileSize;
            int tileY = (tileIndex / worldTilesPerRow) * worldSourceTileSize;

            Rectangle sourceRec = { (float)tileX + worldPadding, (float)tileY + worldPadding,
                                    (float)worldSourceTileSize - 2 * worldPadding, (float)worldSourceTileSize - 2 * worldPadding };

            Rectangle destRec = { (float)worldX, (float)worldY, (float)worldTileSize, (float)worldTileSize };
            DrawTexturePro(tilemap, sourceRec, destRec, {0, 0}, 0.0f, WHITE);
        }
    }
}

void Chunk::DrawObjects(Texture2D& tilemap) const {
    for (const Object& obj : objects) {
        if (obj.type == ObjectType::OBJECT_NONE) continue;

        int objectTileIndex = objectTypeToTile(obj.type);
        int objTileX = (objectTileIndex % worldTilesPerRow) * worldSourceTileSize;
        int objTileY = (objectTileIndex / worldTilesPerRow) * worldSourceTileSize;

        Rectangle objSource = { (float)objTileX, (float)objTileY, (float)worldSourceTileSize, (float)worldSourceTileSize };

        auto it = objectPropertiesMap.find(obj.type);
        float widthPx = worldTileSize;
        float heightPx = worldTileSize;
        if (it != objectPropertiesMap.end()) {
            widthPx = it->second.size.x * worldTileSize;
            heightPx = it->second.size.y * worldTileSize;
        }

        Rectangle destRec = { obj.position.x, obj.position.y, widthPx, heightPx };
        DrawTexturePro(tilemap, objSource, destRec, {0, 0}, 0.0f, WHITE);
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
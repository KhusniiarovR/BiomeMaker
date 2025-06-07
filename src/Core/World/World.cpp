#include "World.h"
#include "Constants/GraphicsConst.h"
#include <ranges>
#include "Constants/WorldConst.h"

void World::update(Vector2 playerPos) {
    const int chunkPixelSize = CHUNK_SIZE * TILE_SIZE;

    int playerChunkX = static_cast<int>(playerPos.x / chunkPixelSize);
    int playerChunkY = static_cast<int>(playerPos.y / chunkPixelSize);

    const int preloadAhead = RENDER_DISTANCE / 2;
    const int preloadBehind = RENDER_DISTANCE - preloadAhead;

    int loadStartX = playerChunkX - preloadBehind;
    int loadEndX   = playerChunkX + preloadAhead + 1;

    int loadStartY = playerChunkY - preloadBehind;
    int loadEndY   = playerChunkY + preloadAhead + 1;

    for (int y = loadStartY; y < loadEndY; ++y) {
        for (int x = loadStartX; x < loadEndX; ++x) {
            //if (x < 0 || y < 0 || x >= WORLD_CHUNKS || y >= WORLD_CHUNKS) continue;

            auto key = std::make_pair(x, y);
            if (chunks.find(key) == chunks.end()) {
                chunks.emplace(key, Chunk(x, y));
            }
        }
    }

    int unloadStartX = loadStartX - unloadMargin;
    int unloadEndX   = loadEndX + unloadMargin;
    int unloadStartY = loadStartY - unloadMargin;
    int unloadEndY   = loadEndY + unloadMargin;

    auto it = chunks.begin();
    while (it != chunks.end()) {
        auto [chunkX, chunkY] = it->first;
        if (chunkX < unloadStartX || chunkX >= unloadEndX ||
            chunkY < unloadStartY || chunkY >= unloadEndY) {
            it = chunks.erase(it);
            } else {
                ++it;
            }
    }
}

void World::render() const {
    for (const auto& chunk : chunks | std::views::values) {
        chunk.Draw();
    }
}

int World::getChunkCount() const {
    return chunks.size();
}
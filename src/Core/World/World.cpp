#include "World.h"
#include "Constants/GraphicsConst.h"
#include <ranges>

void World::update(Vector2 playerPos) {
    int chunkPixelSize = CHUNK_SIZE * TILE_SIZE;

    int loadStartX = static_cast<int>((playerPos.x - screenSizeX / 2.0f) / chunkPixelSize) - 1;
    int loadStartY = static_cast<int>((playerPos.y - screenSizeY / 2.0f) / chunkPixelSize) - 1;
    int loadEndX = loadStartX + RENDER_DISTANCE;
    int loadEndY = loadStartY + RENDER_DISTANCE;

    for (int y = loadStartY; y < loadEndY; y++) {
        for (int x = loadStartX; x < loadEndX; x++) {
            auto key = std::make_pair(x, y);
            if (chunks.find(key) == chunks.end()) {
                chunks.emplace(key, Chunk(x, y));
            }
        }
    }

    int unloadStartX = loadStartX - 1;
    int unloadEndX = loadEndX + 1;
    int unloadStartY = loadStartY - 1;
    int unloadEndY = loadEndY + 1;

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
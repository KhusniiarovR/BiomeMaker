#include "World.h"
#include "Constants/GraphicsConst.h"
#include <ranges>
#include "Constants/WorldConst.h"

World::World(const std::string &filename) : filename(filename) {
    LoadHeaders();
}

void World::LoadHeaders() {
    worldFile.open(filename + "/world.dat", std::ios::binary);
    if (!worldFile.is_open()) {
        std::cerr << "Error: can't open world.dat\n";
        return;
    }

    int totalChunks = WORLD_SIZE;
    headers.resize(totalChunks);
    worldFile.read(reinterpret_cast<char*>(headers.data()), totalChunks * sizeof(ChunkHeader));
}

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
            auto key = std::make_pair(x, y);
            if (chunks.find(key) == chunks.end()) {
                chunks.emplace(key, Chunk(x, y, headers, worldFile));
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
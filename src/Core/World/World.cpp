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

    int totalChunks = worldSize;
    headers.resize(totalChunks);
    worldFile.read(reinterpret_cast<char*>(headers.data()), totalChunks * sizeof(ChunkHeader));
}

void World::update(Vector2 playerPos, Camera2D& camera) {
    updateChunks(playerPos);

    if (IsKeyPressed(KEY_E)) {
    Vector2 mouseScreen = GetMousePosition();
    Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);

    int tileX = static_cast<int>(mouseWorld.x / tileSize);
    int tileY = static_cast<int>(mouseWorld.y / tileSize);

    int playerTileX = static_cast<int>(playerPos.x / tileSize);
    int playerTileY = static_cast<int>(playerPos.y / tileSize);

    int dx = tileX - playerTileX;
    int dy = tileY - playerTileY;

    if (dx * dx + dy * dy <= handDistance*handDistance) {
        removeObjectAt(tileX, tileY);
    }
}
}

void World::render(Renderer& renderer) const {
    Texture2D& tilemap = renderer.getTexture("tilemap");
    for (const auto& chunk : chunks | std::views::values) {
        chunk.Draw(tilemap);
    }
}

void World::updateChunks(Vector2 playerPos) {
    const int chunkPixelSize = chunkSize * tileSize;

    int playerChunkX = static_cast<int>(playerPos.x / chunkPixelSize);
    int playerChunkY = static_cast<int>(playerPos.y / chunkPixelSize);

    const int preloadAhead = renderDistance / 2;
    const int preloadBehind = renderDistance - preloadAhead;

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

bool World::removeObjectAt(int worldX, int worldY) {
    int chunkX = worldX / chunkSize;
    int chunkY = worldY / chunkSize;

    int localX = worldX % chunkSize;
    int localY = worldY % chunkSize;

    auto it = chunks.find({chunkX, chunkY});
    if (it == chunks.end()) return false;

    Chunk& chunk = it->second;
    Object& obj = chunk.objectTiles[localY][localX];

    if (obj.type != ObjectType::None) {
        obj.type = ObjectType::None;
        return false;
    }
    return true;
}

int World::getChunkCount() const {
    return chunks.size();
}
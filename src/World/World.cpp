#include "World.h"
#include "Constants/GraphicsConst.h"
#include <ranges>
#include "Constants/WorldConst.h"
#include <sstream>

World::World(const std::string &filename) : chunkSystem(chunks, filename) {}

World::~World() = default;

void World::update(Vector2 playerPos, Camera2D& camera) {
    chunkSystem.update(playerPos);

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
    chunkSystem.render(renderer);
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
        chunks.at({chunkX, chunkY}).isModified = true; 
        return true;
    }
    return false;
}

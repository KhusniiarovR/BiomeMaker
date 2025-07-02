#include "World.h"
#include "Constants/WorldConst.h"
#include "Constants/TilemapConst.h"
#include <optional>

World::World(const std::string &filename) : chunkSystem(chunks, filename) {}

World::~World() = default;

void World::update(Vector2 playerPos, Camera2D& camera, Vector2 mouseVirtual) {
    chunkSystem.update(playerPos);
}

void World::render(Renderer& renderer) const {
    chunkSystem.render(renderer);
}

Object* World::getObjectAt(int worldX, int worldY) {
    int chunkX = worldX / chunkSize;
    int chunkY = worldY / chunkSize;

    int localX = worldX % chunkSize;
    int localY = worldY % chunkSize;

    auto it = chunks.find({chunkX, chunkY});
    if (it == chunks.end()) return nullptr;

    Chunk& chunk = it->second;
    return &chunk.objectTiles[localY][localX];
}

std::optional<ObjectType> World::removeObjectAt(int worldX, int worldY) {
    Object* obj = getObjectAt(worldX, worldY);

    if (!obj || obj->type == ObjectType::None) return std::nullopt;
    
    ObjectType removedType = obj->type;
    obj->type = ObjectType::None;

    int chunkX = worldX / chunkSize;
    int chunkY = worldY / chunkSize;
    chunks.at({chunkX, chunkY}).isModified = true;   

    return removedType;
}

bool World::placeObjectAt(int worldX, int worldY, ObjectType type) {
    if (type == ObjectType::None)
        return false;

    int chunkX = worldX / chunkSize;
    int chunkY = worldY / chunkSize;

    int localX = worldX % chunkSize;
    int localY = worldY % chunkSize;

    if (localX < 0) { chunkX--; localX += chunkSize; }
    if (localY < 0) { chunkY--; localY += chunkSize; }

    auto it = chunks.find({chunkX, chunkY});
    if (it == chunks.end()) return false;

    Chunk& chunk = it->second;
    Object& obj = chunk.objectTiles[localY][localX];

    if (obj.type != ObjectType::None)
        return false;

    obj.type = type;
    chunk.isModified = true;

    return true;
}
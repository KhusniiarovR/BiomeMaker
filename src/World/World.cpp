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

std::optional<ObjectType> World::removeObjectAt(int worldX, int worldY) {
    int chunkX = worldX / chunkSize;
    int chunkY = worldY / chunkSize;

    auto it = chunks.find({chunkX, chunkY});
    if (it == chunks.end()) return std::nullopt;

    Chunk& chunk = it->second;

    auto found = std::find_if(chunk.objects.begin(), chunk.objects.end(), [&](const Object& obj) {
        auto prop = objectPropertiesMap.find(obj.type);
        if (prop == objectPropertiesMap.end()) return false;

        Vector2 size = prop->second.size;
        int startX = static_cast<int>(obj.position.x / worldTileSize);
        int startY = static_cast<int>(obj.position.y / worldTileSize);

        return worldX >= startX && worldX < startX + (int)size.x &&
               worldY >= startY && worldY < startY + (int)size.y;
    });

    if (found == chunk.objects.end()) return std::nullopt;

    ObjectType removedType = found->type;
    chunk.objects.erase(found);
    chunk.isModified = true;
    return removedType;
}

bool World::placeObjectAt(int worldX, int worldY, ObjectType type) {
    if (type == ObjectType::OBJECT_NONE)
        return false;

    int chunkX = worldX / chunkSize;
    int chunkY = worldY / chunkSize;

    int localX = worldX % chunkSize;
    int localY = worldY % chunkSize;

    auto it = chunks.find({chunkX, chunkY});
    if (it == chunks.end()) return false;

    Chunk& chunk = it->second;

    auto propIt = objectPropertiesMap.find(type);
    if (propIt == objectPropertiesMap.end()) return false;

    int w = static_cast<int>(propIt->second.size.x);
    int h = static_cast<int>(propIt->second.size.y);

    if (localX + w > chunkSize || localY + h > chunkSize)
        return false;

    for (const Object& obj : chunk.objects) {
        auto otherPropIt = objectPropertiesMap.find(obj.type);
        if (otherPropIt == objectPropertiesMap.end()) continue;

        int objTileX = static_cast<int>(obj.position.x / worldTileSize);
        int objTileY = static_cast<int>(obj.position.y / worldTileSize);

        int objLocalX = objTileX - chunkX * chunkSize;
        int objLocalY = objTileY - chunkY * chunkSize;

        int ow = static_cast<int>(otherPropIt->second.size.x);
        int oh = static_cast<int>(otherPropIt->second.size.y);

        bool overlap = !(localX + w <= objLocalX ||    
                         localX >= objLocalX + ow ||   
                         localY + h <= objLocalY ||    
                         localY >= objLocalY + oh); 

        if (overlap)
            return false;
    }

    Object newObj;
    newObj.type = type;
    newObj.position = { static_cast<float>(chunkX * chunkSize + localX) * worldTileSize,
                        static_cast<float>(chunkY * chunkSize + localY) * worldTileSize };

    chunk.objects.push_back(newObj);
    chunk.isModified = true;
    return true;
}

std::vector<Object> World::getObjectsAll() const {
    std::vector<Object> result;

    for (const auto& [pos, chunk] : chunks) {
        result.insert(result.end(), chunk.objects.begin(), chunk.objects.end());
    }

    return result;
}
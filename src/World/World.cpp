#include "World.h"
#include "Constants/WorldConst.h"
#include "Constants/TilemapConst.h"
#include <optional>
#include <algorithm>

World::World(const std::string &filename) : chunkSystem(chunks, filename) {}

World::~World() = default;

void World::update(Vector2 playerPos, Camera2D& camera, Vector2 mouseVirtual) 
{
    chunkSystem.update(playerPos);
}

void World::render(Renderer& renderer) const 
{
    chunkSystem.render(renderer);
}

std::optional<ObjectType> World::removeObjectAt(int worldX, int worldY, const Item* tool) 
{
    // finding chunk
    int chunkX = worldX / chunkSize;
    int chunkY = worldY / chunkSize;

    auto it = chunks.find({chunkX, chunkY});
    if (it == chunks.end()) return std::nullopt;

    Chunk& chunk = it->second;

    // finding object
    auto found = std::find_if(chunk.getObjects().begin(), chunk.getObjects().end(), [&](const Object& obj) 
    {
        auto propertiesIt = objectPropertiesMap.find(obj.type);
        if (propertiesIt == objectPropertiesMap.end()) { return false; }

        const ObjectProperties& properties = propertiesIt->second;

        // is player hitting object
        int objStartX = static_cast<int>(obj.position.x / worldTileSize);
        int objStartY = static_cast<int>(obj.position.y / worldTileSize);
        int objEndX = objStartX + static_cast<int>(properties.size.x);
        int objEndY = objStartY + static_cast<int>(properties.size.y);

        bool inBounds = worldX >= objStartX && worldX < objEndX && worldY >= objStartY && worldY < objEndY;

        if (!inBounds) { return false; }
        return tool->canBreak(properties);
    });

    
    if (found == chunk.getObjects().end()) { return std::nullopt; }

    // erase if find
    ObjectType removedType = found->type;
    chunk.getObjects().erase(found);
    chunk.setModified(true);

    return removedType;
}

bool World::placeObjectAt(int worldX, int worldY, ObjectType type) 
{
    if (type == ObjectType::OBJECT_NONE) { return false; }

    auto propertiesIt = objectPropertiesMap.find(type);
    if (propertiesIt == objectPropertiesMap.end()) return false;

    const ObjectProperties& properties = propertiesIt->second;

    int chunkX = worldX / chunkSize;
    int chunkY = worldY / chunkSize;

    int localX = worldX % chunkSize;
    int localY = worldY % chunkSize;

    // is object inside chunk
    if (localX + static_cast<int>(properties.size.x) > chunkSize || localY + static_cast<int>(properties.size.y) > chunkSize) { return false; }

    // finding chunk
    auto it = chunks.find({chunkX, chunkY});
    if (it == chunks.end()) { return false; }

    Chunk& chunk = it->second;

    // check if object already placed here
    for (const Object& obj : chunk.getObjects()) 
    {
        auto otherPropIt = objectPropertiesMap.find(obj.type);
        if (otherPropIt == objectPropertiesMap.end()) { continue; }

        const ObjectProperties& otherProp = otherPropIt->second;

        int otherX = static_cast<int>(obj.position.x / worldTileSize);
        int otherY = static_cast<int>(obj.position.y / worldTileSize);

        bool overlap = !(worldX + static_cast<int>(properties.size.x) <= otherX ||
                         worldX >= otherX + static_cast<int>(otherProp.size.x) ||
                         worldY + static_cast<int>(properties.size.y) <= otherY ||
                         worldY >= otherY + static_cast<int>(otherProp.size.y));

        if (overlap) { return false; }
    }

    Object newObj = {type, {static_cast<float>(worldX) * worldTileSize, static_cast<float>(worldY) * worldTileSize}};
    chunk.getObjects().push_back(newObj);
    chunk.setModified(true);

    return true;
}

std::vector<Object> World::getObjectsAll() const 
{
    std::vector<Object> result;
    for (const auto& [pos, chunk] : chunks) { result.insert(result.end(), chunk.getObjects().begin(), chunk.getObjects().end()); }

    return result;
}
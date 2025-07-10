#ifndef WORLD_H
#define WORLD_H

#include "ChunkSystem/Chunk.h"
#include "Utilities/World/PairHash.h"
#include <unordered_map>
#include "Core/Render/Renderer.h"
#include "ChunkSystem/ChunkSystem.h"
#include "Items/ItemBase/Item.h"
#include <optional>

class World {
private:
    std::unordered_map<std::pair<int, int>, Chunk, PairHash> chunks;
    ChunkSystem chunkSystem;

public:
    World(const std::string &filename);
    ~World();

    void update(Vector2 playerPos, Camera2D& camera, Vector2 mouseVirtual);
    void render(Renderer& renderer) const;

    std::optional<ObjectType> removeObjectAt(int worldX, int worldY, const Item* tool);
    bool placeObjectAt(int worldX, int worldY, ObjectType type);
    std::vector<Object> getObjectsAll() const;

};

#endif //WORLD_H

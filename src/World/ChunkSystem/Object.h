#pragma once
#include <cstdint>
#include <unordered_map>
#include <vector>
#include "raylib.h"
#include "Biome.h"

enum class ObjectType : uint8_t {
    NONE = 0,
    TREE,
    ROCK,
    BUSH,
    FLOWER,
    WOODEN_WALL
};

enum class ToolType : uint8_t {
    NONE,
    AXE,
    PICKAXE
};

struct ObjectProperties {
    Vector2 visualSize = {1, 1};
    Rectangle hitbox = {0, 0, 0, 0};
    bool hasCollision = false;
    ToolType requiredTool = ToolType::NONE;
    int requiredTier = 0;
};

inline const std::unordered_map<ObjectType, ObjectProperties> objectPropertiesMap = { 
    { ObjectType::TREE,        {{2, 2}, {0.75f, 1.0f, 0.5f, 0.5f}, true, ToolType::AXE, 1} },
    { ObjectType::ROCK,        {{1, 1}, {0.1f, 0.1f, 0.8f, 0.8f}, true, ToolType::PICKAXE, 1} },
    { ObjectType::BUSH,        {{1, 1}, {}, false} },
    { ObjectType::FLOWER,      {{1, 1}, {}, false} },
    { ObjectType::WOODEN_WALL, {{1, 1}, {0.0f, 0.0f, 1.0f, 1.0f}, true, ToolType::AXE, 1} }
};

struct Object {
    ObjectType type = ObjectType::NONE;
    Vector2 position = {0, 0};

    Rectangle getHitbox(float tileSize) const 
    {
        const auto* props = getProperties();
        if (!props) return {0, 0, 0, 0};

        return { position.x + props->hitbox.x * tileSize, position.y + props->hitbox.y * tileSize, 
                 props->hitbox.width * tileSize, props->hitbox.height * tileSize };
    }

    Rectangle getHitboxVisual(float tileSize) const 
    {
        const auto* props = getProperties();
        if (!props) return {0, 0, 0, 0};

        return { position.x, position.y, props->visualSize.x * tileSize, props->visualSize.y * tileSize };
    }

    bool hasCollision() const 
    {
        const auto* props = getProperties();
        return props && props->hasCollision;
    }

    bool isValid() const { return type != ObjectType::NONE; }

    const ObjectProperties* getProperties() const 
    {
        auto it = objectPropertiesMap.find(type);
        return (it != objectPropertiesMap.end()) ? &it->second : nullptr;
    }
};

struct ObjectRule {
    float probability;
    ObjectType type;
};

inline const std::unordered_map<uint8_t, std::vector<ObjectRule>> objectRules = {
    { BIOME_FOREST, { {0.03f, ObjectType::TREE}, {0.005f, ObjectType::BUSH} } },
    { BIOME_WINTER, { {0.003f, ObjectType::ROCK} } },                      
    { BIOME_SAND,   { {0.01f, ObjectType::TREE} } },
    { BIOME_SWAMP,  { {0.01f, ObjectType::FLOWER} } }                        
};

inline const std::unordered_map<ObjectType, int> objectTileMap = {
    { ObjectType::TREE, 60 },
    { ObjectType::ROCK, 62 },
    { ObjectType::BUSH, 63 },
    { ObjectType::FLOWER, 64 },
    { ObjectType::WOODEN_WALL, 65 }
};

#pragma pack(push, 1)
struct FileObject {
    uint8_t type;
    uint8_t localX;
    uint8_t localY;
};
#pragma pack(pop)

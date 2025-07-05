#ifndef OBJECT_H
#define OBJECT_H

#include <cstdint>
#include <unordered_map>
#include <vector>
#include "raylib.h"
#include "Biome.h"

enum ObjectType : uint8_t {
    OBJECT_NONE = 0,
    OBJECT_TREE = 1,
    OBJECT_ROCK = 2,
    OBJECT_BUSH = 3,
    OBJECT_FLOWER = 4,
    OBJECT_WOODEN_WALL = 5
};

struct ObjectProperties {
    Vector2 size;
    bool hasCollision;
};

inline const std::unordered_map<ObjectType, ObjectProperties> objectPropertiesMap = {
    { OBJECT_TREE,        {{2, 2}, true} },
    { OBJECT_ROCK,        {{1, 1}, true}  },
    { OBJECT_BUSH,        {{1, 1}, false} },
    { OBJECT_FLOWER,      {{1, 1}, false} },
    { OBJECT_WOODEN_WALL, {{1, 1}, true}  }
};

struct Object {
    ObjectType type = OBJECT_NONE;
    Vector2 position = {0, 0};

    Rectangle getBoundingBox(float tileSize) const {
        auto it = objectPropertiesMap.find(type);
        if (it == objectPropertiesMap.end()) return {0, 0, 0, 0};

        Vector2 size = { it->second.size.x * tileSize, it->second.size.y * tileSize };
        return { position.x, position.y, size.x, size.y };
    }

    bool hasCollision() const {
        auto it = objectPropertiesMap.find(type);
        return (it != objectPropertiesMap.end()) ? it->second.hasCollision : false;
    }

    bool isValid() const {
        return type != OBJECT_NONE;
    }
};

struct ObjectRule {
    float probability;
    uint8_t type;
};

const std::unordered_map<uint8_t, std::vector<ObjectRule>> objectRules = {
    { BIOME_FOREST, { {0.03f, OBJECT_TREE}, {0.005f, OBJECT_BUSH} } },
    { BIOME_WINTER, { {0.003f,OBJECT_ROCK} } },                      
    { BIOME_SAND,   { {0.01f, OBJECT_TREE} } },
    { BIOME_SWAMP,   { {0.01f,OBJECT_FLOWER} } }                        
};

const std::unordered_map<ObjectType, int> objectTileMap = {
    { ObjectType::OBJECT_TREE, 60 },
    { ObjectType::OBJECT_ROCK, 61 },
    { ObjectType::OBJECT_BUSH, 62 },
    { ObjectType::OBJECT_FLOWER, 63},
    { ObjectType::OBJECT_WOODEN_WALL, 64},
};

#pragma pack(push, 1)
struct FileObject {
    uint8_t type;
    uint8_t localX;
    uint8_t localY;
};
#pragma pack(pop)

#endif // OBJECT_H
#ifndef OBJECT_H
#define OBJECT_H

#include <unordered_map>
#include <vector>
#include <cstdint>
#include "Biome.h"

constexpr uint8_t OBJECT_NONE   = 0;
constexpr uint8_t OBJECT_TREE   = 1;
constexpr uint8_t OBJECT_ROCK   = 2;
constexpr uint8_t OBJECT_BUSH   = 3;
constexpr uint8_t OBJECT_FLOWER = 4;
constexpr uint8_t OBJECT_WOODEN_WALL = 5;

enum class ObjectType : uint8_t {
    None = OBJECT_NONE,
    Tree = OBJECT_TREE,
    Rock = OBJECT_ROCK,
    Bush = OBJECT_BUSH,
    Flower = OBJECT_FLOWER,
    WoodenWall = OBJECT_WOODEN_WALL
};

struct Object {
    ObjectType type = ObjectType::None;
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
    { ObjectType::Tree, 60 },
    { ObjectType::Rock, 61 },
    { ObjectType::Bush, 62 },
    { ObjectType::Flower, 63},
    { ObjectType::WoodenWall, 64},
};

#endif // OBJECT_H
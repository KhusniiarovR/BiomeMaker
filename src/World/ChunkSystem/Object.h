#ifndef OBJECT_H
#define OBJECT_H

#include <unordered_map>

struct ObjectRule {
    float probability;
    char symbol;
};

enum class ObjectType : uint8_t {
    None,
    Tree,
    Rock,
    Bush
};

struct Object {
    ObjectType type = ObjectType::None;
};


const std::unordered_map<char, std::vector<ObjectRule>> objectRules = {
    { 'F', { {0.1f, 'T'}, {0.005f, 'B'} } },
    { 'W', { {0.003f, 'R'} } },
    { 'S', { {0.01f, 'T'} } },
};    

const std::unordered_map<ObjectType, int> objectTileMap = {
    { ObjectType::Tree, 60 },
    { ObjectType::Rock, 61 },
    { ObjectType::Bush, 62 }
};


#endif // OBJECT_H
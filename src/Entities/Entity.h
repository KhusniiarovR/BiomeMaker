#pragma once
#include <raylib.h>

class Entity { // basic entity class
public:
    Vector2 position;

    Entity(Vector2 initPos) : position(initPos) {}
    virtual ~Entity() = default;
    
    virtual void update(float dt) = 0; // for logic
    virtual Rectangle getBoundingBox() const = 0; // for collisions
};
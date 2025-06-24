#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>

class Entity {
public:
    Vector2 position;
    virtual void update(float dt) = 0;
    Entity(Vector2 initPos) : position(initPos) {}
    virtual ~Entity() = default;
};

#endif //ENTITY_H

#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>

class Entity {
public:
    Vector2 position;

    Entity(Vector2 initPos) : position(initPos) {}
    virtual ~Entity() = default;
    
    virtual void update(float dt) = 0;
    virtual Rectangle getBoundingBox() const = 0;
};

#endif //ENTITY_H

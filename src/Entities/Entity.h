#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>

class Entity {
public:
    Vector2 position;
    virtual void update(float dt) = 0;
    virtual void draw() const = 0;
    virtual ~Entity() = default;
};

#endif //ENTITY_H

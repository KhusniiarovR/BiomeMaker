#ifndef COLLISIONBASE_H
#define COLLISIONBASE_H

#include "raylib.h"

class CollisionBase {
public:
    virtual bool checkCollision(Rectangle rect) const = 0;
    virtual ~CollisionBase() = default;
};

#endif // COLLISIONBASE_H
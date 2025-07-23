#pragma once
#include "raylib.h"

class CollisionBase { // abstract collision class
public:
    virtual bool checkCollision(Rectangle rect) const = 0;
    virtual ~CollisionBase() = default;
};
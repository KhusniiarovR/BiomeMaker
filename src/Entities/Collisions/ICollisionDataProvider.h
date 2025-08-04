#pragma once
#include "raylib.h"
#include <vector>

class ICollisionDataProvider { // interface for collision class
public:
    virtual ~ICollisionDataProvider() = default;

    virtual Rectangle getPlayerHitbox() const = 0;
    virtual std::vector<Rectangle> getEnemyHitboxes() const = 0;
    virtual std::vector<Rectangle> getWorldObjectHitboxes() const = 0;
};

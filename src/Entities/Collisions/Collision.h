#pragma once
#include "ICollisionDataProvider.h"

class Collision { // class that knows about every hitbox
private:
    const ICollisionDataProvider* provider = nullptr;

public:
    void setProvider(const ICollisionDataProvider* p) { provider = p; }

    bool checkWithWorld(Rectangle rect) const {
        for (const auto& box : provider->getWorldObjectHitboxes()) 
        {
            if (CheckCollisionRecs(rect, box)) { return true; }
        }
        return false;
    }

    bool checkWithEnemies(Rectangle rect) const {
        for (const auto& box : provider->getEnemyHitboxes()) 
        {
            if (CheckCollisionRecs(rect, box)) { return true; }
        }
        return false;
    }

    bool checkWithPlayer(Rectangle rect) const { return CheckCollisionRecs(rect, provider->getPlayerHitbox()); }
};

#ifndef COLLISIONWORLD_H
#define COLLISIONWORLD_H

#include "CollisionBase.h"
#include "World/World.h"
#include "Constants/TilemapConst.h"

class CollisionWorld : public CollisionBase {
private:
    const World& world;

public:
    CollisionWorld(const World& world) : world(world) {}

    bool checkCollision(Rectangle rect) const override {
        const auto& objects = world.getObjectsAll();
        for (const Object& obj : objects) {
            if (!obj.hasCollision()) continue;
            Rectangle objBox = obj.getBoundingBox(worldTileSize);
            if (CheckCollisionRecs(rect, objBox)) return true;
        }
        return false;
    }
};

#endif // COLLISIONWORLD_H
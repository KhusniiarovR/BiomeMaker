#ifndef ITEMUSECONTEXT_H
#define ITEMUSECONTEXT_H

#include "raylib.h"
#include "World/World.h"
#include "Entities/Player.h"

struct ItemUseContext {
    World& world;
    Player& player;
    Vector2 mouseWorld;
    int tileX, tileY;

    ItemUseContext(World& world, Player& player, Vector2 mouseVirtual, Camera2D camera, int tileSize)
        : world(world), player(player) {
        mouseWorld = GetScreenToWorld2D(mouseVirtual, camera);
        tileX = static_cast<int>(mouseWorld.x) / tileSize;
        tileY = static_cast<int>(mouseWorld.y) / tileSize;
    }
};

#endif // ITEMUSECONTEXT_H
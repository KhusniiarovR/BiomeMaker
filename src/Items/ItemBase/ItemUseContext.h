#pragma once
#include "raylib.h"
#include "World/World.h"
#include "Entities/Player/Player.h"
#include "Constants/TilemapConst.h"

struct ItemUseContext { // has world, player and mouse
    World& world;
    Player& player;
    Vector2 mouseWorld;
    int tileX, tileY;

    ItemUseContext(World& world, Player& player, Vector2 mouseVirtual, Camera2D camera) : world(world), player(player) 
    {
        mouseWorld = GetScreenToWorld2D(mouseVirtual, camera);
        tileX = static_cast<int>(mouseWorld.x) / worldTileSize;
        tileY = static_cast<int>(mouseWorld.y) / worldTileSize;
    }
};
#pragma once
#include "Items/ItemBase/Item.h"
#include "World/ChunkSystem/Object.h"
#include "Items/ItemBase/ItemUseContext.h"
#include "raymath.h"

struct ItemPlaceable : public Item // class for blocks
{
    ObjectType objectType; // what will be places

    ItemPlaceable(ObjectType type) : objectType(type) {}
    bool isPlaceable() const override { return true; }

    bool onUse(const ItemUseContext& context) const override 
    {
        Rectangle tileBounds = { (float) context.tileX * worldTileSize, (float) context.tileY * worldTileSize, worldTileSize, worldTileSize };
        Vector2 playerPos = context.player.getPosition();
        float maxDist = context.player.getHandDistance() * worldTileSize;
        if (!CheckCollisionCircleRec(playerPos, maxDist, tileBounds)) return false;
        
        return context.world.placeObjectAt(context.tileX, context.tileY, objectType);
    }
};
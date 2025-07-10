#ifndef ITEMPLACEABLE_H
#define ITEMPLACEABLE_H

#include "Items/ItemBase/Item.h"
#include "World/ChunkSystem/Object.h"
#include "Items/ItemBase/ItemUseContext.h"
#include "raymath.h"

struct ItemPlaceable : public Item {
    ObjectType objectType;

    ItemPlaceable(ObjectType type) : objectType(type) {}

    bool isPlaceable() const override { return true; }

    bool onUse(const ItemUseContext& context) const override {
        Vector2 playerPos = context.player.getPosition();
        Vector2 playerCenter = Vector2Add(playerPos, { entityTileSize / 2.0f, entityTileSize / 2.0f });
        Vector2 tilePos = { context.tileX * worldTileSize + worldTileSize / 2.0f,
                            context.tileY * worldTileSize + worldTileSize / 2.0f };
        float distSq = Vector2DistanceSqr(playerCenter, tilePos);
        float maxDist = context.player.getHandDistance() * worldTileSize;
        float maxDistSq = maxDist * maxDist;

        if (distSq > maxDistSq) { return false; }
        return context.world.placeObjectAt(context.tileX, context.tileY, objectType);
    }
};


#endif // ITEMPLACEABLE_H
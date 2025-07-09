#ifndef ITEMTOOL_H
#define ITEMTOOL_H

#include "Items/ItemBase/Item.h"
#include "Items/ItemBase/ObjectToItem.h"
#include "raymath.h"
#include "Constants/TilemapConst.h"
#include "Constants/WorldConst.h"
#include "Items/ItemBase/ItemUseContext.h"

struct ItemToolBase : public Item {
    int maxDurability;

    ItemToolBase(ItemID id, std::string name, std::string description, int durability)
        : Item(id, std::move(name), std::move(description)),
          maxDurability(durability)
    {
        stackable = false;
        maxStack = 1;
    }

    bool onUse(const ItemUseContext& context) const override {
        Vector2 playerPos = context.player.getPosition();
        Vector2 playerCenter = Vector2Add(playerPos, { entityTileSize / 2.0f, entityTileSize / 2.0f });
        Vector2 tilePos = { context.tileX * worldTileSize + worldTileSize / 2.0f,
                            context.tileY * worldTileSize + worldTileSize / 2.0f };
        float distSq = Vector2DistanceSqr(playerCenter, tilePos);
        float maxDist = context.player.getHandDistance() * worldTileSize;
        float maxDistSq = maxDist * maxDist;

        if (distSq > maxDistSq) { return false; }

        auto removed = context.world.removeObjectAt(context.tileX, context.tileY, this);
        if (removed) {
            std::vector<ItemID> drops = generateLootForObject(*removed);
            for (ItemID id : drops) {
                context.player.giveItem(id, 1);
            }
            return true;
        }

        return false;
    }

    bool canBreak(ObjectType type) const override {
        return false;
    }

    bool shouldConsumeOnUse() const override {
        return false;
    }
};

#endif // ITEMTOOL_H
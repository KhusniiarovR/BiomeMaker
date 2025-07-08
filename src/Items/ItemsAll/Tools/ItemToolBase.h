#ifndef ITEMTOOL_H
#define ITEMTOOL_H

#include "Items/ItemBase/Item.h"
#include "Items/ItemBase/ItemUseContext.h"
#include "Items/ItemBase/ObjectToItem.h"
#include "raymath.h"
#include "Constants/TilemapConst.h"
#include "Constants/WorldConst.h"

struct ItemToolBase : public Item {
    int maxDurability;
    int currentDurability;

    ItemToolBase(ItemID id, std::string name, std::string description, int durability)
        : Item(id, std::move(name), std::move(description)),
          maxDurability(durability),
          currentDurability(durability)
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

            // currentDurability--;

            return true;
        }

        return false;
    }


    virtual bool canBreak(ObjectType type) const {
        return false;
    }

    bool shouldConsumeOnUse() const override {
        return false;
    }

    virtual int getDurability() const { return currentDurability; }
    float getDurabilityRatio() const {return maxDurability > 0 ? (float)currentDurability / maxDurability : 0.0f;}
    void damage(int amount = 1) {
        if (currentDurability > amount) {currentDurability -= amount;}
        else {currentDurability = 0;}
    }
};



#endif // ITEMTOOL_H
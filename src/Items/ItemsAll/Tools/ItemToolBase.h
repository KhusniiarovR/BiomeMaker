#ifndef ITEMTOOL_H
#define ITEMTOOL_H

#include "Items/ItemBase/Item.h"
#include "Items/ItemBase/ObjectToItem.h"
#include "raymath.h"
#include "Constants/TilemapConst.h"
#include "Constants/WorldConst.h"
#include "Items/ItemBase/ItemUseContext.h"
#include "World/ChunkSystem/Object.h"

struct ItemToolBase : public Item {
    int maxDurability;
    ToolType type = ToolType::NONE;
    int tier = 0;

    ItemToolBase(ItemID id, std::string name, std::string description, ToolType type, int tier, int durability)
        : Item(id, std::move(name), std::move(description)),
          type(type), tier(tier), maxDurability(durability)
    {
        stackable = false;
        maxStack = 1;
    }

    bool canBreak(const ObjectProperties& objProp) const override {
        if (objProp.requiredTool == ToolType::NONE) return true;
        return objProp.requiredTool == type && tier >= objProp.requiredTier;
    }

    bool shouldConsumeOnUse() const override {
        return false;
    }

    ToolType getToolType() const { return type; }
    int getToolTier() const { return tier; }
};

#endif // ITEMTOOL_H
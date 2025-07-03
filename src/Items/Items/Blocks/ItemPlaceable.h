#ifndef ITEMPLACEABLE_H
#define ITEMPLACEABLE_H

#include "Items/ItemBase/Item.h"
#include "World/ChunkSystem/Object.h"
#include "Items/ItemBase/ItemUseContext.h"

struct ItemPlaceable : public Item {
    ObjectType objectType;

    ItemPlaceable(ObjectType type) : objectType(type) {}

    bool isPlaceable() const override { return true; }

    bool onUse(const ItemUseContext& context) const override {
        return context.world.placeObjectAt(context.tileX, context.tileY, objectType);
    }
};





#endif // ITEMPLACEABLE_H
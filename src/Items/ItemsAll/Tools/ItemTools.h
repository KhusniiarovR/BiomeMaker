#ifndef ITEMTOOLS_H
#define ITEMTOOLS_H

#include "Items/ItemBase/Item.h"
#include "ItemToolBase.h"

struct ItemAxe : public ItemToolBase {
    ItemAxe();
    bool canBreak(ObjectType type) const override;
};

struct ItemPickaxe : public ItemToolBase {
    ItemPickaxe();
    bool canBreak(ObjectType type) const override;
};
 
#endif // ITEMTOOLS_H
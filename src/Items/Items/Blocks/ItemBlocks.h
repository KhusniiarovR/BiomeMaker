#ifndef ITEMBLOCKS_H
#define ITEMBLOCKS_H

#include "Items/ItemBase/Item.h"

struct ItemWood : public Item {
    ItemWood() {
        id = ItemID::WOOD;
        name = "Wood";
        description = "dropped from tree";
    }
};

struct ItemStone : public Item {
    ItemStone() {
        id = ItemID::STONE;
        name = "Stone";
        description = "dropped from big stone";
    }
};

#endif // ITEMBLOCKS_H
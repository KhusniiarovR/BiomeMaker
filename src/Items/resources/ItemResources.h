#ifndef ITEMRESOURCES_H
#define ITEMRESOURCES_H

#include "Items/Item.h"

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

struct ItemBerry : public Item {
    ItemBerry() {
        id = ItemID::BERRY;
        name = "Berry";
        description = "looks tasty";
    }
};



#endif // ITEMRESOURCES_H
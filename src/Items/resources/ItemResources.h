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



#endif // ITEMRESOURCES_H
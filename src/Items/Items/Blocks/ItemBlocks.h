#ifndef ITEMBLOCKS_H
#define ITEMBLOCKS_H

#include "Items/ItemBase/Item.h"
#include "ItemPlaceable.h"

struct ItemWood : public ItemPlaceable {
    ItemWood();
};

struct ItemStone : public ItemPlaceable {
    ItemStone();
};
 
#endif // ITEMBLOCKS_H
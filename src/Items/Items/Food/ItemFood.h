#ifndef ITEMFOOD_H
#define ITEMFOOD_H

#include "Items/Item.h"
#include "Entities/Player.h"

struct ItemBerry : public Item {
    ItemBerry();

    void onUse(Player& player) const override;
};

#endif // ITEMFOOD_H
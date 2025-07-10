#ifndef ITEMSTACK_H
#define ITEMSTACK_H

#include "Item.h"
#include "Items/ItemRegister/ItemRegister.h"

struct ItemStack {
    ItemID id = ItemID::NONE;
    uint8_t count = 0;
    uint16_t durability = 0;

    Item& getItem() const;

    bool isEmpty() const;

    int getMaxDurability() const;
    float getDurabilityRatio() const;
    void damage(int amount = 1);
    bool isBroken() const;
};

#endif // ITEMSTACK_H
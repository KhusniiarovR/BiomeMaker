#pragma once
#include "Item.h"
#include "Items/ItemRegister/ItemRegister.h"

struct ItemStack { // one inventory slot that have id and amount(durability)
    ItemID id = ItemID::NONE;
    uint8_t count = 0;
    uint16_t durability = 0;

    bool isEmpty() const;
    Item& getItem() const;
    int getMaxDurability() const;
    float getDurabilityRatio() const;
    void damage(int amount = 1);
    bool isBroken() const;
};
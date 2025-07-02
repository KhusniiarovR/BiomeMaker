#ifndef ITEMSTACK_H
#define ITEMSTACK_H

#include "Item.h"
#include "Items/ItemRegister/ItemRegister.h"

struct ItemStack {
    ItemID id = ItemID::NONE;
    uint8_t count = 0;

    bool isEmpty() const { return id == ItemID::NONE || count == 0; }

    const Item& getItem() const {
        return ItemRegister::get().getItem(id);
    }
};


#endif // ITEMSTACK_H
#ifndef INVENTORY_H
#define INVENTORY_H

#include <array>
#include "Items/ItemStack.h"

class Inventory {
public:
    static const int SLOT_COUNT = 20;
    int selectedSlot = 0;

    Inventory();

    ItemStack& getSlot(int index);
    const ItemStack& getSlot(int index) const;
    void setSelectedSlot(int index);
    const ItemStack& getSelectedSlot() const;

    bool addItem(ItemID id, uint8_t count);

private:
    std::array<ItemStack, SLOT_COUNT> slots;
};


#endif // INVENTORY_H
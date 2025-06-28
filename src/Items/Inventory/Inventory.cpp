#include "Inventory.h"

Inventory::Inventory() {
    for (auto& slot : slots) {
        slot.id = ItemID::NONE;
        slot.count = 0;
    }
}

ItemStack& Inventory::getSlot(int index) {
    return slots[index];
}

const ItemStack& Inventory::getSlot(int index) const {
    return slots[index];
}

bool Inventory::addItem(ItemID id, uint8_t count) {
    for (auto& slot : slots) {
        if (slot.id == id && slot.count < slot.getItem().maxStack) {
            uint8_t space = slot.getItem().maxStack - slot.count;
            uint8_t toAdd = std::min(count, space);
            slot.count += toAdd;
            count -= toAdd;
            if (count == 0) return true;
        }
    }

    for (auto& slot : slots) {
        if (slot.isEmpty()) {
            slot.id = id;
            slot.count = count;
            return true;
        }
    }

    return false; 
}

#include "ItemStack.h"
#include "Items/ItemsAll/Tools/ItemToolBase.h"

bool ItemStack::isEmpty() const { return id == ItemID::NONE || (count == 0 && durability == 0); }

Item& ItemStack::getItem() const {
    return ItemRegister::get().getItem(id);
}

int ItemStack::getMaxDurability() const {
    const Item& item = getItem();
    if (const auto* tool = dynamic_cast<const ItemToolBase*>(&item)) {
        return tool->maxDurability;
    }
    return 0;
}

float ItemStack::getDurabilityRatio() const {
    int maxDur = getMaxDurability();
    return maxDur > 0 ? (float)durability / maxDur : 0.f;
}

void ItemStack::damage(int amount) {
    if (durability > amount) {
        durability -= amount;
    } else {
        durability = 0;
    }
}
bool ItemStack::isBroken() const {
    return durability == 0;
}
#include "itemRegister.h"
#include "Items/ItemsAll/Blocks/ItemBlocks.h"
#include "Items/ItemsAll/Food/ItemFood.h"

ItemRegister& ItemRegister::get() {
    static ItemRegister instance;
    return instance;
}

void ItemRegister::registerItem(Item* item) {
    items[item->id] = std::unique_ptr<Item>(item);
}

Item& ItemRegister::getItem(ItemID id) const {
    return *items.at(id);
}

bool ItemRegister::hasItem(ItemID id) const {
    return items.find(id) != items.end();
}
#include "itemRegister.h"
#include "Items/Items/Blocks/ItemBlocks.h"
#include "Items/Items/Food/ItemFood.h"

ItemRegister& ItemRegister::get() {
    static ItemRegister instance;
    return instance;
}

void ItemRegister::registerItem(Item* item) {
    items[item->id] = std::unique_ptr<Item>(item);
}

const Item& ItemRegister::getItem(ItemID id) const {
    return *items.at(id);
}
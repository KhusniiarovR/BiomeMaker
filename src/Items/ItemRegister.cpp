#include "itemRegister.h"
#include "Items/resources/ItemResources.h"

void registerAllResources();


void registerAllItems() {
    registerAllResources();
}

ItemRegister& ItemRegister::get() {
    static ItemRegister instance;
    return instance;
}

void ItemRegister::registerItem(std::unique_ptr<Item> item) {
    items[item->id] = std::move(item);
}

const Item& ItemRegister::getItem(ItemID id) const {
    auto it = items.find(id);
    if (it != items.end()) return *it->second;
    static Item unknown = { ItemID::NONE, "Unknown", "???" };
    return unknown;
}

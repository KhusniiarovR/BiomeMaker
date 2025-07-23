#pragma once
#include <unordered_map>
#include <memory>
#include "Items/ItemBase/Item.h"

class ItemRegister {
public:
    static ItemRegister& get();

    void registerItem(Item* item);
    Item& getItem(ItemID id) const;
    bool hasItem(ItemID id) const;

private:
    std::unordered_map<ItemID, std::unique_ptr<Item>> items;
};
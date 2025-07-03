#ifndef ITEMREGISTER_H
#define ITEMREGISTER_H

#include <unordered_map>
#include <memory>
#include "Items/ItemBase/Item.h"

class ItemRegister {
public:
    static ItemRegister& get();

    void registerItem(Item* item);
    const Item& getItem(ItemID id) const;

private:
    std::unordered_map<ItemID, std::unique_ptr<Item>> items;
};

void registerAllItems();

#endif // ITEMREGISTER_H
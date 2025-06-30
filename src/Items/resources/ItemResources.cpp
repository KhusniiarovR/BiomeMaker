#include "Items/ItemRegister.h"
#include "ItemResources.h"

void registerAllResources() {
    ItemRegister::get().registerItem(std::make_unique<ItemWood>());
    ItemRegister::get().registerItem(std::make_unique<ItemStone>());
    ItemRegister::get().registerItem(std::make_unique<ItemBerry>());
}
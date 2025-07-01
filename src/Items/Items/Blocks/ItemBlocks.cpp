#include "Items/ItemRegister.h"
#include "ItemBlocks.h"

void registerAllBlocks() {
    ItemRegister::get().registerItem(std::make_unique<ItemWood>());
    ItemRegister::get().registerItem(std::make_unique<ItemStone>());
}
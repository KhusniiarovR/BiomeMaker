#include "ItemRegisterAll.h"
#include "Items/Items/Blocks/ItemBlocks.h"
#include "Items/Items/Food/ItemFood.h"

void registerAllFood() {
    ItemRegister::get().registerItem(new ItemBerry());
}

void registerAllBlocks() {
    ItemRegister::get().registerItem(new ItemWood());
    ItemRegister::get().registerItem(new ItemStone());
}

void registerAllItems() {
    registerAllFood();
    registerAllBlocks();
}
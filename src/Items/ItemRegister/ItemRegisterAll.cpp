#include "ItemRegisterAll.h"
#include "Items/ItemsAll/Blocks/ItemBlocks.h"
#include "Items/ItemsAll/Food/ItemFood.h"
#include "Items/ItemsAll/Tools/ItemTools.h"

void registerAllFood() {
    ItemRegister::get().registerItem(new ItemBerry());
}

void registerAllBlocks() {
    ItemRegister::get().registerItem(new ItemWood());
    ItemRegister::get().registerItem(new ItemStone());
}

void registerAllTools() {
    ItemRegister::get().registerItem(new ItemAxe());
    ItemRegister::get().registerItem(new ItemPickaxe());
}

void registerAllItems() {
    registerAllFood();
    registerAllBlocks();
    registerAllTools();
}
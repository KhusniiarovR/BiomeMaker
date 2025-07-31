#pragma once
#include <cstdint>

enum class ItemID : uint16_t // list of all items
{
    NONE = 0,
    WOOD = 1,
    STONE = 2,
    BERRY = 3,
    AXE = 4,
    PICKAXE = 5
};

/*
to add item 
1. add item name here intro ItemId
2. go to Items/ItemsAll/ and search for your group of items(blocks, food, tools)
3. create new struct based on existing ones
4. go to Items/ItemRegister/ItemRegisterAll.cpp and register new item there
*/
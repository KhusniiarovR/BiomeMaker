#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "raylib.h"
#include "World/ChunkSystem/Object.h"
#include "Constants/TilemapConst.h"
#include "Constants/WorldConst.h"
#include "Items/ItemsAll/ItemList.h" // to add item go here

class ItemUseContext; // forward declaration

struct Item { // base item class
    ItemID id; // item unique number
    std::string name; // short item name
    std::string description; // long explanation
    bool stackable = true; // can be more than 1 in slot
    uint8_t maxStack = 10; // if can max amount

    Item() = default;
    Item(ItemID id, std::string name, std::string description);
    virtual ~Item();

    static constexpr int ICON_SIZE = 8; // icon image size
    static constexpr int ICONS_PER_ROW = 10; // icons in one row

    Rectangle getIconSourceRect() const; // position and size if icon

    virtual bool onUse(const ItemUseContext& context) const; // method for logic for every item
    virtual bool isPlaceable() const; // for blocks
    virtual bool shouldConsumeOnUse() const; // for food
    virtual bool canBreak(const ObjectProperties& objProp) const; // what tool can break
};
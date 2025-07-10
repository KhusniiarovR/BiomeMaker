#ifndef ITEM_H
#define ITEM_H

#include <cstdint>
#include <string>
#include <vector>
#include "raylib.h"
#include "World/ChunkSystem/Object.h"
#include "Constants/TilemapConst.h"
#include "Constants/WorldConst.h"

class ItemUseContext;

enum class ItemID : uint16_t {
    NONE = 0,
    WOOD = 1,
    STONE = 2,
    BERRY = 3,
    AXE = 4,
    PICKAXE = 5
};

struct Item {
    ItemID id;
    std::string name;
    std::string description;
    bool stackable = true;
    uint8_t maxStack = 10;

    Item() = default;
    Item(ItemID id, std::string name, std::string description);
    virtual ~Item();

    static constexpr int ICON_SIZE = 8;
    static constexpr int ICONS_PER_ROW = 10;

    Rectangle getIconSourceRect() const;

    virtual bool onUse(const ItemUseContext& context) const;
    virtual bool isPlaceable() const;
    virtual bool shouldConsumeOnUse() const;
    virtual bool canBreak(const ObjectProperties& objProp) const;
};

#endif // ITEM_H
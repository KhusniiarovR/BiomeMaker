#ifndef ITEM_H
#define ITEM_H

#include <cstdint>
#include <string>
#include "raylib.h"
#include <functional>

class ItemUseContext;

enum class ItemID : uint16_t {
    NONE = 0,
    WOOD = 1,
    STONE = 2,
    BERRY = 3,
};

struct Item {
    ItemID id;
    std::string name;
    std::string description;
    bool stackable = true;
    uint8_t maxStack = 10;

    Item() = default;

    Item(ItemID id, std::string name, std::string description)
        : id(id), name(std::move(name)), description(std::move(description)) {}

    static constexpr int ICON_SIZE = 8;
    static constexpr int ICONS_PER_ROW = 10;
        
    Rectangle getIconSourceRect() const {
        int index = static_cast<int>(id);
        int x = (index % ICONS_PER_ROW) * ICON_SIZE;
        int y = (index / ICONS_PER_ROW) * ICON_SIZE;
        return { (float)x, (float)y, (float)ICON_SIZE, (float)ICON_SIZE };
    }

    virtual ~Item() = default;

    virtual bool onUse(const ItemUseContext& context) const { return false; }
    virtual bool isPlaceable() const { return false; }
    virtual bool shouldConsumeOnUse() const { return true; }
};

#endif // ITEM_H
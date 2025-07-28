#pragma once
#include <array>
#include "Items/ItemBase/ItemStack.h"
#include "Core/Render/Renderer.h"
#include "Constants/TilemapConst.h"

class Inventory {
private:
    std::array<ItemStack, slotCount> slots; // all items in inventory
    int hoveredSlot = -1; // for mouse hovering
    int selectedSlot = 0; // which item is in hand
    std::string fileName; // inventory file name

public:
    Inventory(std::string fileName);
    ~Inventory();
    void update(Vector2 mouseVirtual, bool full);
    void render(Renderer& renderer, bool full) const;

    /* getters and setters */
    ItemStack& getSlot(int index);
    const ItemStack& getSlot(int index) const;
    void setSelectedSlot(int index);
    const ItemStack& getSelectedSlot() const;
    ItemStack& getSelectedSlot();
    int getHoveredSlot();
    bool addItem(ItemID id, uint8_t count);
    
private:
    /* save and load from file */
    void save() const;
    bool load();
};
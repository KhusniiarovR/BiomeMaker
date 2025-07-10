#ifndef INVENTORY_H
#define INVENTORY_H

#include <array>
#include "Items/ItemBase/ItemStack.h"
#include "Core/Render/Renderer.h"
#include "Constants/TilemapConst.h"

class Inventory {
public:
    int selectedSlot = 0;
    int hoveredSlot = -1; 
    std::string fileName;

    Inventory(std::string fileName);

    ItemStack& getSlot(int index);
    const ItemStack& getSlot(int index) const;
    void setSelectedSlot(int index);
    const ItemStack& getSelectedSlot() const;

    void update(Vector2 mouseVirtual);
    void render(Renderer& renderer) const;

    bool addItem(ItemID id, uint8_t count);

    void save() const;
    bool load();

private:
    std::array<ItemStack, slotCount> slots;
};


#endif // INVENTORY_H
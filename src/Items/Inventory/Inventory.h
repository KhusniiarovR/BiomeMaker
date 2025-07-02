#ifndef INVENTORY_H
#define INVENTORY_H

#include <array>
#include "Items/ItemBase/ItemStack.h"
#include "Core/Render/Renderer.h"

class Inventory {
public:
    static const int SLOT_COUNT = 20;
    int selectedSlot = 0;
    int hoveredSlot = -1; 

    Inventory();

    ItemStack& getSlot(int index);
    const ItemStack& getSlot(int index) const;
    void setSelectedSlot(int index);
    const ItemStack& getSelectedSlot() const;

    void update(Vector2 mouseVirtual);
    void render(Renderer& renderer) const;

    bool addItem(ItemID id, uint8_t count);

private:
    std::array<ItemStack, SLOT_COUNT> slots;
};


#endif // INVENTORY_H
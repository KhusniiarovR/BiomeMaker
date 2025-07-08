#include "Inventory.h"
#include "Constants/TilemapConst.h"
#include "Items/ItemsAll/Tools/ItemToolBase.h"

Inventory::Inventory() {
    for (auto& slot : slots) {
        slot.id = ItemID::NONE;
        slot.count = 0;
    }
}

ItemStack& Inventory::getSlot(int index) {
    return slots[index];
}

const ItemStack& Inventory::getSlot(int index) const {
    return slots[index];
}

void Inventory::setSelectedSlot(int index) {
    if (index >= 0 && index < SLOT_COUNT)
        selectedSlot = index;
}
    
const ItemStack& Inventory::getSelectedSlot() const {
    return slots[selectedSlot];
}

void Inventory::update(Vector2 mouseVirtual) {
    hoveredSlot = -1;
    for (int i = 0; i < SLOT_COUNT; i++) {
        int col = i % invColumns;
        int row = i / invColumns;

        float slotX = invPosition.x + col * (invSlotSize + invPadding);
        float slotY = invPosition.y + row * (invSlotSize + invPadding);
        Rectangle slotRect = { slotX, slotY, (float)invSlotSize, (float)invSlotSize };

        if (CheckCollisionPointRec(mouseVirtual, slotRect)) {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            hoveredSlot = i;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                setSelectedSlot(i);
                break;
            }
        }
    }
}

void Inventory::render(Renderer& renderer) const {
    Texture2D& itemTilemap = renderer.getTexture("itemTilemap");

    for (int i = 0; i < Inventory::SLOT_COUNT; ++i) {
        int col = i % invColumns;
        int row = i / invColumns;

        float x = invPosition.x + col * (invSlotSize + invPadding);
        float y = invPosition.y + row * (invSlotSize + invPadding);
        Rectangle slotRect = { x, y, (float)invSlotSize, (float)invSlotSize };

        DrawRectangleRec(slotRect, DARKGRAY);
        DrawRectangleLinesEx(slotRect, 2, (i == selectedSlot) ? RED : LIGHTGRAY);

        const ItemStack& stack = getSlot(i);
        if (!stack.isEmpty()) {
            const Item& item = stack.getItem();

            float iconSize = invSlotSize * 0.8f;
            float iconX = x + (invSlotSize - iconSize) * 0.5f;
            float iconY = y + (invSlotSize - iconSize) * 0.5f;

            Rectangle src = item.getIconSourceRect();
            Rectangle dst = { iconX, iconY, iconSize, iconSize };

            DrawTexturePro(itemTilemap, src, dst, {0, 0}, 0.0f, WHITE);

            const ItemToolBase* tool = dynamic_cast<const ItemToolBase*>(&item);
            if (tool) {
                float durability = tool->getDurabilityRatio();

                Color barColor = (durability > 0.60f) ? GREEN :
                                (durability > 0.25f) ? YELLOW : RED;

                float barPadding = 2.0f;
                float barHeight = 3.0f;
                float barWidth = invSlotSize - barPadding * 2;

                float barX = x + barPadding;
                float barY = y + invSlotSize - barHeight - barPadding;

                Rectangle backBar = { barX, barY, barWidth, barHeight };
                Rectangle fillBar = { barX, barY, barWidth * durability, barHeight };

                DrawRectangleRec(backBar, GRAY);
                DrawRectangleRec(fillBar, barColor);
                DrawRectangleLinesEx(backBar, 1, DARKGRAY);
            }

            if (stack.count > 1) {
                renderer.drawText(std::to_string(stack.count), {x + invSlotSize-5,  y + invSlotSize-5}, 16, BLACK, true, false);
            }
        }
    }

    if (hoveredSlot != -1) {
        const ItemStack& stack = getSlot(hoveredSlot);
        if (!stack.isEmpty()) {
            const Item& item = stack.getItem();

            float descX = invPosition.x;
            float descY = invPosition.y + invRows * (invSlotSize + invPadding) + 5;

            renderer.drawText(item.description, {descX, descY}, 16, DARKGRAY, false, false);
        }
    }
}
// todo inventory saving

bool Inventory::addItem(ItemID id, uint8_t count) {
    for (auto& slot : slots) {
        if (slot.id == id && slot.count < slot.getItem().maxStack) {
            uint8_t space = slot.getItem().maxStack - slot.count;
            uint8_t toAdd = std::min(count, space);
            slot.count += toAdd;
            count -= toAdd;
            if (count == 0) return true;
        }
    }

    for (auto& slot : slots) {
        if (slot.isEmpty()) {
            slot.id = id;
            slot.count = count;
            return true;
        }
    }

    return false; 
}

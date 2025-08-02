#include "Inventory.h"
#include <fstream>
#include "Items/ItemsAll/Tools/ItemToolBase.h"
#include "Items/ItemRegister/ItemRegister.h"

Inventory::Inventory(std::string fileName) : fileName(fileName + "/inventory.inv") 
{
    load();
}

Inventory::~Inventory() 
{
    save();
}

void Inventory::update(Vector2 mouseVirtual, bool full) 
{
    // select slot with number keys
    for (int i = 0; i < std::min(9, invColumns); i++) { if (IsKeyPressed(KEY_ONE + i)) { setSelectedSlot(i); } }
    // 10th slot check
    if (10 >= invColumns && IsKeyPressed(KEY_ZERO)) { setSelectedSlot(9); }

    // select slot with mouse scroll
    float wheel = GetMouseWheelMove();
    if (wheel != 0) 
    {
        int maxSlots = full ? slotCount : std::min(10, slotCount);
        selectedSlot -= static_cast<int>(wheel);
        if (selectedSlot < 0) { selectedSlot = maxSlots - 1; }
        if (selectedSlot >= maxSlots) { selectedSlot = 0; }
    }

    // mouse hovering and selection check
    hoveredSlot = -1;
    int slotsToCheck = full ? slotCount : std::min(10, slotCount);
    for (int i = 0; i < slotsToCheck; i++) 
    {
        int col = i % invColumns;
        int row = i / invColumns;
        
        float slotX = invPosition.x + col * (invSlotSize + invPadding);
        float slotY = invPosition.y + row * (invSlotSize + invPadding);
        Rectangle slotRect = { slotX, slotY, (float)invSlotSize, (float)invSlotSize };
        
        if (CheckCollisionPointRec(mouseVirtual, slotRect)) 
        {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            hoveredSlot = i;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
            {
                setSelectedSlot(i);
                break;
            }
        }
    }
}

void Inventory::render(Renderer& renderer, bool full) const 
{
    /* render inventory */
    Texture2D& itemTilemap = renderer.getTexture("itemTilemap");
    
    int slotsToDraw = full ? slotCount : std::min(10, slotCount); // render full or not
    int rowsToDraw = (slotsToDraw + invColumns - 1) / invColumns;
    
    // render slot
    for (int i = 0; i < slotsToDraw; ++i) 
    {
        int col = i % invColumns;
        int row = i / invColumns;
        
        float x = invPosition.x + col * (invSlotSize + invPadding);
        float y = invPosition.y + row * (invSlotSize + invPadding);
        Rectangle slotRect = { x, y, (float)invSlotSize, (float)invSlotSize };
        
        DrawRectangleRec(slotRect, DARKGRAY);
        DrawRectangleLinesEx(slotRect, 2, (i == selectedSlot) ? RED : LIGHTGRAY);
        
        // render item
        const ItemStack& stack = getSlot(i);
        if (!stack.isEmpty()) 
        {
            const Item& item = stack.getItem();
            
            float iconSize = invSlotSize * 0.8f;
            float iconX = x + (invSlotSize - iconSize) * 0.5f;
            float iconY = y + (invSlotSize - iconSize) * 0.5f;

            Rectangle src = item.getIconSourceRect();
            Rectangle dst = { iconX, iconY, iconSize, iconSize };
            
            DrawTexturePro(itemTilemap, src, dst, {0, 0}, 0.0f, WHITE);
            
            // render durability for tools
            const ItemToolBase* tool = dynamic_cast<const ItemToolBase*>(&item);
            if (tool) 
            {
                float durability = stack.getDurabilityRatio();
                
                Color barColor = (durability > 0.60f) ? GREEN : (durability > 0.25f) ? YELLOW : RED;
                float barPadding = 2.0f;
                float barHeight = 4.0f;
                float barWidth = invSlotSize - barPadding * 2;
                
                float barX = x + barPadding;
                float barY = y + invSlotSize - barHeight - barPadding;
                
                Rectangle backBar = { barX, barY, barWidth, barHeight };
                Rectangle fillBar = { barX, barY, barWidth * durability, barHeight };
                
                DrawRectangleRec(backBar, GRAY);
                DrawRectangleRec(fillBar, barColor);
                DrawRectangleLinesEx(backBar, 1, DARKGRAY);
            }
            
            // render items amount
            if (stack.count > 1) { renderer.drawText(std::to_string(stack.count), {x + invSlotSize-5,  y + invSlotSize-5}, 16, BLACK, true, false); }
        }
    }
    
    // render item description if hovered
    if (full && hoveredSlot != -1) 
    {
        const ItemStack& stack = getSlot(hoveredSlot);
        if (!stack.isEmpty()) 
        {
            const Item& item = stack.getItem();
            
            float descX = invPosition.x;
            float descY = invPosition.y + rowsToDraw * (invSlotSize + invPadding) + 5;
            
            renderer.drawText(item.description, {descX, descY}, 16, DARKGRAY, false, false);
        }
    }
}

bool Inventory::addItem(ItemID id, uint8_t count) 
{
    for (auto& slot : slots) // check if item copy is already in inventory
    {
        if (slot.id == id && slot.count < slot.getItem().maxStack) 
        {
            uint8_t space = slot.getItem().maxStack - slot.count;
            uint8_t toAdd = std::min(count, space);
            slot.count += toAdd;
            count -= toAdd;
            if (count == 0) { return true; }
        }
    }
    for (auto& slot : slots) // try to find empty slot
    {
        if (slot.isEmpty()) 
        {
            slot.id = id;
            slot.count = count;
            slot.durability = slot.getMaxDurability();
            return true;
        }
    }
    return false; 
}

void Inventory::save() const 
{
    // saving as pair id + amount(durability)
    std::ofstream out(fileName, std::ios::binary);
    if (!out) return;
    
    for (int i = 0; i < slotCount; ++i) 
    {
        const ItemStack& stack = slots[i];
        if (!ItemRegister::get().hasItem(stack.id)) // if slot is empty
        {
            uint16_t noneId = static_cast<uint16_t>(ItemID::NONE);
            out.write(reinterpret_cast<const char*>(&noneId), sizeof(noneId));
            uint8_t zeroCount = 0;
            out.write(reinterpret_cast<const char*>(&zeroCount), sizeof(zeroCount));
            continue;
        }
        
        uint16_t id = static_cast<uint16_t>(stack.id);
        out.write(reinterpret_cast<const char*>(&id), sizeof(id));
        const Item& item = ItemRegister::get().getItem(stack.id);
        if (item.stackable) { out.write(reinterpret_cast<const char*>(&stack.count), sizeof(stack.count)); } 
        else { out.write(reinterpret_cast<const char*>(&stack.durability), sizeof(stack.durability)); }
    }
}

bool Inventory::load() 
{
    // loading as pair id + amount(durability)
    std::ifstream in(fileName, std::ios::binary);
    if (!in) return false;
    
    for (int i = 0; i < slotCount; ++i) 
    {
        uint16_t idRaw = 0;
        if (!in.read(reinterpret_cast<char*>(&idRaw), sizeof(idRaw))) // checks amount of pairs
        {
            for (int j = i; j < slotCount; ++j) { slots[j] = ItemStack{}; }
            return false;
        }
        
        ItemID id = static_cast<ItemID>(idRaw); // ttying to transform first number to item id
        if (!ItemRegister::get().hasItem(id)) 
        {
            for (int j = i; j < slotCount; ++j) { slots[j] = ItemStack{}; }
            return false;
        }
        slots[i].id = id;
        const Item& item = ItemRegister::get().getItem(id);
        
        // use second number as amount or durability
        if (item.stackable) 
        {
            uint8_t count = 0;
            if (!in.read(reinterpret_cast<char*>(&count), sizeof(count))) 
            {
                for (int j = i; j < slotCount; ++j) { slots[j] = ItemStack{}; }
                return false;
            }
            slots[i].count = count;
            slots[i].durability = 0;
        } 
        else 
        {
            uint16_t durability = 0;
            if (!in.read(reinterpret_cast<char*>(&durability), sizeof(durability))) 
            {
                for (int j = i; j < slotCount; ++j) { slots[j] = ItemStack{}; }
                return false;
            }
            slots[i].durability = durability;
            slots[i].count = 1;
        }
    }
    return true;
}

int Inventory::getHoveredSlot() 
{
    return hoveredSlot;
}

ItemStack& Inventory::getSlot(int index) 
{
    return slots[index];
}

const ItemStack& Inventory::getSlot(int index) const 
{
    return slots[index];
}

void Inventory::setSelectedSlot(int index) 
{
    if (index >= 0 && index < slotCount) { selectedSlot = index; }
}
    
const ItemStack& Inventory::getSelectedSlot() const 
{
    return slots[selectedSlot];
}

ItemStack& Inventory::getSelectedSlot() 
{
    return slots[selectedSlot];
}
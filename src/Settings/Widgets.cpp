#include "Widgets.h"
#include <algorithm>
#include "SettingsConstants.h"
#include "Utilities/Input/KeysTranslate.h"

/*   SectionHeader   */

SectionHeader::SectionHeader(const std::string& t) : title(t) {}

void SectionHeader::render(Renderer& renderer, float y) const 
{
    renderer.drawText(title.c_str(), {20.0f, y}, SECTION_TITLE_SIZE, COLOR_SECTION_TITLE, false, false, "silkscreen", 0.0f);
}

int SectionHeader::getHeight() const 
{
    return SECTION_TITLE_SIZE + SECTION_SPACING;
}

/*   IntField   */

IntField::IntField(const std::string& l, int* v, int minV, int maxV) : label(l), value(v), min(minV), max(maxV) {}

void IntField::update(Vector2 mouseVirtual, float y) 
{
    Rectangle fieldRect = { LABEL_WIDTH, y, FIELD_WIDTH, FIELD_HEIGHT };

    if (CheckCollisionPointRec(mouseVirtual, fieldRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
    {
        editing = true;
        buffer = std::to_string(*value);
    }

    if (editing) 
    {
        int key = GetCharPressed();
        while (key > 0) 
        {
            if (key >= '0' && key <= '9' && buffer.size() < 6) buffer += (char)key;
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !buffer.empty()) { buffer.pop_back(); } // todo text box functions

        if (IsKeyPressed(KEY_ENTER)) 
        {
            if (!buffer.empty()) { *value = std::clamp(std::stoi(buffer), min, max); }
            editing = false;
            buffer.clear();
        }
    }

    Rectangle plusBtn = { fieldRect.x + FIELD_WIDTH + BUTTON_SPACING, y, BUTTON_WIDTH, FIELD_HEIGHT };
    Rectangle minusBtn = { plusBtn.x + BUTTON_WIDTH + BUTTON_SPACING, y, BUTTON_WIDTH, FIELD_HEIGHT };

    if (CheckCollisionPointRec(mouseVirtual, plusBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { *value = std::min(*value + 10, max); }
    if (CheckCollisionPointRec(mouseVirtual, minusBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { *value = std::max(*value - 10, min); }
}

void IntField::render(Renderer& renderer, float y) const 
{
    renderer.drawText(label.c_str(), {20.0f, y}, TEXT_SIZE, COLOR_TEXT, false, false, "silkscreen", 1.0f);

    Rectangle fieldRect = { LABEL_WIDTH, y, FIELD_WIDTH, FIELD_HEIGHT };
    Color bgColor = editing ? COLOR_BG_EDITING : COLOR_BG_DEFAULT;
    DrawRectangleRec(fieldRect, bgColor);

    std::string text = editing ? buffer : std::to_string(*value);
    renderer.drawText(text.c_str(), {fieldRect.x + 10.0f, fieldRect.y + 5.0f}, TEXT_SIZE, COLOR_TEXT_FIELD, false, false, "silkscreen", 2.0f);

    DrawRectangle(fieldRect.x + FIELD_WIDTH + BUTTON_SPACING, y, BUTTON_WIDTH, FIELD_HEIGHT, COLOR_PLUS_MINUS_BTN);
    DrawRectangle(fieldRect.x + FIELD_WIDTH + BUTTON_SPACING * 2 + BUTTON_WIDTH, y, BUTTON_WIDTH, FIELD_HEIGHT, COLOR_PLUS_MINUS_BTN);
    renderer.drawText("+", {fieldRect.x + FIELD_WIDTH + BUTTON_SPACING + 8, y + 5}, TEXT_SIZE, COLOR_TEXT, false, false);
    renderer.drawText("-", {fieldRect.x + FIELD_WIDTH + BUTTON_SPACING * 2 + BUTTON_WIDTH + 8, y + 5}, TEXT_SIZE, COLOR_TEXT, false, false);
}

int IntField::getHeight() const 
{
    return LINE_HEIGHT;
}

/*   KeyBindField   */

KeyBindField::KeyBindField(Action a) : action(a) {}

void KeyBindField::update(Vector2 mouseVirtual, float y) 
{
    Rectangle btn = { LABEL_WIDTH, y, width, FIELD_HEIGHT };

    if (!waitingForKey && CheckCollisionPointRec(mouseVirtual, btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { waitingForKey = true; }

    if (waitingForKey) 
    {
        for (int k = 32; k < 349; ++k) 
        {
            if (IsKeyPressed(k)) 
            {
                InputManager::GetInstance().SetKeyBinding(action, k);
                waitingForKey = false;
                break;
            }
        }
    }
}

void KeyBindField::render(Renderer& renderer, float y) const 
{
    renderer.drawText(InputManager::GetActionName(action), {20.0f, y}, TEXT_SIZE + 5, COLOR_TEXT, false, false, "silkscreen", 1.0f);

    Rectangle btn = { LABEL_WIDTH, y, width, FIELD_HEIGHT };
    DrawRectangleRec(btn, COLOR_KEYBIND_BTN);

    std::string keyName = waitingForKey ? "..." : GetKeyName(InputManager::GetInstance().GetKeyBinding(action));
    renderer.drawText(keyName.c_str(), {btn.x + 5.0f, btn.y}, TEXT_SIZE+10, COLOR_TEXT, false, false);
}

int KeyBindField::getHeight() const 
{
    return LINE_HEIGHT;
}
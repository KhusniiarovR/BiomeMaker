#include "Widgets.h"
#include <algorithm>
#include "SettingsConstants.h"
#include "Utilities/Input/KeysTranslate.h"

SectionHeader::SectionHeader(const std::string& t) : title(t) {}

void SectionHeader::Render(float y) const {
    DrawText(title.c_str(), 20, (int)y, SECTION_TITLE_SIZE, COLOR_SECTION_TITLE);
}

int SectionHeader::GetHeight() const {
    return SECTION_TITLE_SIZE + SECTION_SPACING;
}

IntField::IntField(const std::string& l, int* v, int minV, int maxV)
    : label(l), value(v), min(minV), max(maxV) {}

void IntField::Update(Vector2 mouseVirtual, float y) {
    Rectangle fieldRect = { LABEL_WIDTH, y, FIELD_WIDTH, FIELD_HEIGHT };

    if (CheckCollisionPointRec(mouseVirtual, fieldRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        editing = true;
        buffer = std::to_string(*value);
    }

    if (editing) {
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= '0' && key <= '9' && buffer.size() < 6) buffer += (char)key;
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !buffer.empty()) {
            buffer.pop_back();
        }

        if (IsKeyPressed(KEY_ENTER)) {
            if (!buffer.empty()) {
                *value = std::clamp(std::stoi(buffer), min, max);
            }
            editing = false;
            buffer.clear();
        }
    }

    Rectangle plusBtn = { fieldRect.x + FIELD_WIDTH + BUTTON_SPACING, y, BUTTON_WIDTH, FIELD_HEIGHT };
    Rectangle minusBtn = { plusBtn.x + BUTTON_WIDTH + BUTTON_SPACING, y, BUTTON_WIDTH, FIELD_HEIGHT };

    if (CheckCollisionPointRec(mouseVirtual, plusBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *value = std::min(*value + 10, max);
    }
    if (CheckCollisionPointRec(mouseVirtual, minusBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *value = std::max(*value - 10, min);
    }
}

void IntField::Render(float y) const {
    DrawText(label.c_str(), 20, (int)y, TEXT_SIZE, COLOR_TEXT);

    Rectangle fieldRect = { LABEL_WIDTH, y, FIELD_WIDTH, FIELD_HEIGHT };
    Color bgColor = editing ? COLOR_BG_EDITING : COLOR_BG_DEFAULT;
    DrawRectangleRec(fieldRect, bgColor);

    std::string text = editing ? buffer : std::to_string(*value);
    DrawText(text.c_str(), fieldRect.x + 10, fieldRect.y + 5, TEXT_SIZE, COLOR_TEXT_FIELD);

    DrawRectangle(fieldRect.x + FIELD_WIDTH + BUTTON_SPACING, y, BUTTON_WIDTH, FIELD_HEIGHT, COLOR_PLUS_MINUS_BTN);
    DrawRectangle(fieldRect.x + FIELD_WIDTH + BUTTON_SPACING * 2 + BUTTON_WIDTH, y, BUTTON_WIDTH, FIELD_HEIGHT, COLOR_PLUS_MINUS_BTN);
    DrawText("+", fieldRect.x + FIELD_WIDTH + BUTTON_SPACING + 8, y + 5, TEXT_SIZE, COLOR_TEXT);
    DrawText("-", fieldRect.x + FIELD_WIDTH + BUTTON_SPACING * 2 + BUTTON_WIDTH + 8, y + 5, TEXT_SIZE, COLOR_TEXT);
}

int IntField::GetHeight() const {
    return LINE_HEIGHT;
}

KeyBindField::KeyBindField(Action a) : action(a) {}

void KeyBindField::Update(Vector2 mouseVirtual, float y) {
    Rectangle btn = { LABEL_WIDTH, y, 100, FIELD_HEIGHT };

    if (!waitingForKey && CheckCollisionPointRec(mouseVirtual, btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        waitingForKey = true;
    }

    if (waitingForKey) {
        for (int k = 32; k < 349; ++k) {
            if (IsKeyPressed(k)) {
                InputManager::GetInstance().SetKeyBinding(action, k);
                waitingForKey = false;
                break;
            }
        }
    }
}

void KeyBindField::Render(float y) const {
    DrawText(InputManager::GetActionName(action), 20, (int)y, TEXT_SIZE, COLOR_TEXT);

    Rectangle btn = { LABEL_WIDTH, y, 100, FIELD_HEIGHT };
    DrawRectangleRec(btn, COLOR_KEYBIND_BTN);

    std::string keyName = waitingForKey ? "..." : GetKeyName(InputManager::GetInstance().GetKeyBinding(action));
    DrawText(keyName.c_str(), btn.x + 10, btn.y + 5, TEXT_SIZE, COLOR_TEXT);
}

int KeyBindField::GetHeight() const {
    return LINE_HEIGHT;
}
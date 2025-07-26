#include "Settings.h"
#include <fstream>
#include "Utilities/Input/KeysTranslate.h"
#include "Utilities/Logger/Logger.h"
#include <algorithm>
#include "SettingsConstants.h"

Settings::Settings(const std::string& path) : configPath(path) {
    Load();
}

Settings::~Settings() {
    Save();
}


void Settings::Load() {
    std::ifstream file(configPath);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        ParseLine(line);
    }
}

void Settings::ParseLine(const std::string& line) {
    size_t eq = line.find('=');
    if (eq == std::string::npos) return;
    std::string key = line.substr(0, eq);
    std::string val = line.substr(eq + 1);

    if (key == "window_width") windowWidth = std::stoi(val);
    else if (key == "window_height") windowHeight = std::stoi(val);
    else if (key == "max_fps") maxFPS = std::stoi(val);
    else {
        int keyCode = GetKeyFromName(val.c_str());
        if (keyCode > 0) {
            actionKeyList.emplace_back(key, keyCode);
            actionKeyMap[key] = keyCode;
        }
    }
}

void Settings::Save() {
    std::ofstream file(configPath);
    file << "#screen\n";
    file << "window_width=" << windowWidth << "\n";
    file << "window_height=" << windowHeight << "\n\n";

    file << "#fps\n";
    file << "max_fps=" << maxFPS << "\n\n";

    file << "#keybinds\n";
    for (const auto& [action, key] : actionKeyList) {
        file << action << "=" << GetKeyName(key) << "\n";
    }

    SetWindowSize(windowWidth, windowHeight);
    SetTargetFPS(maxFPS);
}

void Settings::update(float dt, Vector2 mouseVirtual) {
    float wheel = GetMouseWheelMove();
    scrollOffset -= wheel * SCROLL_SPEED;
    scrollOffset = std::max(scrollOffset, 0.0f); 
    int totalHeight = INITIAL_Y + 3 * LINE_HEIGHT + LINE_HEIGHT + (int)actionKeyList.size() * LINE_HEIGHT;
    int maxScroll = std::max(0, totalHeight - virtualScreenSizeY);
    scrollOffset = std::min(scrollOffset, (float)maxScroll);

    if (waitingForKey) {
        for (int k = 32; k < 349; ++k) {
            if (IsKeyPressed(k)) {
                actionKeyMap[currentBinding] = k;
                waitingForKey = false;
                break;
            }
        }
        return;
    }

    int y = INITIAL_Y;

    UpdateIntField("windowWidth", windowWidth, virtualScreenSizeX, 9999, y, mouseVirtual); y += LINE_HEIGHT;
    UpdateIntField("windowHeight", windowHeight, virtualScreenSizeY, 9999, y, mouseVirtual); y += LINE_HEIGHT;
    UpdateIntField("maxFPS", maxFPS, 30, 1000, y, mouseVirtual); y += LINE_HEIGHT + 10;

    y += LINE_HEIGHT;

    for (const auto& [action, _] : actionKeyList) {
        Rectangle btn = {LABEL_WIDTH, (float)y, 100, FIELD_HEIGHT};
        Rectangle visibleBtn = btn;
        visibleBtn.y -= scrollOffset;

        if (CheckCollisionPointRec(mouseVirtual, visibleBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            waitingForKey = true;
            currentBinding = action;
        }
        y += LINE_HEIGHT;
    }

    if (IsKeyPressed(KEY_Q)) {
        Save();
    }
}

void Settings::render() const {
    BeginScissorMode(0, 0, virtualScreenSizeX, virtualScreenSizeY);

    int y = INITIAL_Y - (int)scrollOffset;
    
    DrawText("Screen: ", 20, y - LINE_HEIGHT, TEXT_SIZE, COLOR_TEXT_GROUP);

    RenderIntField("Window Width", "windowWidth", windowWidth, y); y += LINE_HEIGHT;
    RenderIntField("Window Height", "windowHeight", windowHeight, y); y += LINE_HEIGHT;
    RenderIntField("Max FPS", "maxFPS", maxFPS, y); y += LINE_HEIGHT + 10;

    DrawText("Keybindings:", 20, y, TEXT_SIZE, COLOR_TEXT_GROUP); y += LINE_HEIGHT;

    for (const auto& [action, _] : actionKeyMap) {
        DrawKeyBind(action, y);
        y += LINE_HEIGHT;
    }

    EndScissorMode();
}

void Settings::DrawKeyBind(const std::string& action, int y) const {
    DrawText(action.c_str(), 20, y, TEXT_SIZE, COLOR_TEXT);

    Rectangle btn = {LABEL_WIDTH, (float)y, 100, FIELD_HEIGHT};
    DrawRectangleRec(btn, COLOR_KEYBIND_BTN);

    std::string keyName = (waitingForKey && currentBinding == action) ? "..." : GetKeyName(actionKeyMap.at(action));

    DrawText(keyName.c_str(),  btn.x + 10, btn.y + 5, TEXT_SIZE, COLOR_TEXT);
}

bool Settings::IsActionPressed(const std::string& action) const {
    auto it = actionKeyMap.find(action);
    return it != actionKeyMap.end() ? IsKeyDown(it->second) : false;
}

bool Settings::IsEditingField(const std::string& name) const {
    return editingField == name;
}

void Settings::UpdateIntField(const std::string& fieldName, int& value, int min, int max, int y, Vector2 mouseVirtual) {
    Rectangle fieldRect = {LABEL_WIDTH, (float)y, FIELD_WIDTH, FIELD_HEIGHT};
    Rectangle visibleRect = fieldRect;
    visibleRect.y -= scrollOffset;

    if (CheckCollisionPointRec(mouseVirtual, visibleRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        editingField = fieldName;
        inputBuffer = std::to_string(value);
    }

    if (IsEditingField(fieldName)) {
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= '0' && key <= '9' && inputBuffer.size() < 6) {
                inputBuffer += static_cast<char>(key);
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !inputBuffer.empty()) {
            inputBuffer.pop_back();
        }

        if (IsKeyPressed(KEY_ENTER)) {
            if (!inputBuffer.empty()) {
                int newValue = std::stoi(inputBuffer);
                value = std::clamp(newValue, min, max);
            }
            editingField = "";
            inputBuffer = "";
        }
    }

    Rectangle plusBtn = {LABEL_WIDTH + FIELD_WIDTH + BUTTON_SPACING, (float)y, BUTTON_WIDTH, FIELD_HEIGHT};
    plusBtn.y -= scrollOffset;
    Rectangle minusBtn = {LABEL_WIDTH + FIELD_WIDTH + BUTTON_SPACING * 2 + BUTTON_WIDTH, (float)y, BUTTON_WIDTH, FIELD_HEIGHT};
    minusBtn.y -= scrollOffset;

    if (CheckCollisionPointRec(mouseVirtual, plusBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        value = std::min(value + 10, max);
    }
    if (CheckCollisionPointRec(mouseVirtual, minusBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        value = std::max(value - 10, min);
    }
}

void Settings::RenderIntField(const char* label, const std::string& fieldName, int value, int y) const {
    DrawText(label, 20, y, TEXT_SIZE, COLOR_TEXT);

    Rectangle fieldRect = {LABEL_WIDTH, (float)y, FIELD_WIDTH, FIELD_HEIGHT};
    Color bgColor = IsEditingField(fieldName) ? COLOR_BG_EDITING : COLOR_BG_DEFAULT;
    DrawRectangleRec(fieldRect, bgColor);

    std::string displayText = IsEditingField(fieldName) ? inputBuffer : std::to_string(value);
    DrawText(displayText.c_str(), fieldRect.x + 10, fieldRect.y + 5, TEXT_SIZE, COLOR_TEXT_FIELD);

    DrawRectangle(fieldRect.x + FIELD_WIDTH + BUTTON_SPACING, y, BUTTON_WIDTH, FIELD_HEIGHT, COLOR_PLUS_MINUS_BTN);
    DrawRectangle(fieldRect.x + FIELD_WIDTH + BUTTON_SPACING * 2 + BUTTON_WIDTH, y, BUTTON_WIDTH, FIELD_HEIGHT, COLOR_PLUS_MINUS_BTN);
    DrawText("+", fieldRect.x + FIELD_WIDTH + BUTTON_SPACING + 8, y + 5, TEXT_SIZE, COLOR_TEXT);
    DrawText("-", fieldRect.x + FIELD_WIDTH + BUTTON_SPACING * 2 + BUTTON_WIDTH + 8, y + 5, TEXT_SIZE, COLOR_TEXT);
}
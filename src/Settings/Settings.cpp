#include "Settings.h"
#include <fstream>
#include "Utilities/Input/KeysTranslate.h"
#include "Utilities/Logger/Logger.h"

Settings::Settings(const std::string& path) : configPath(path) {
    // actionNames = {
    //     {GameAction::MoveLeft, "move_left"},
    //     {GameAction::MoveRight, "move_right"},
    //     {GameAction::MoveUp, "move_up"},
    //     {GameAction::MoveDown, "move_down"},
    // };

    // keyMap = {
    //     {GameAction::MoveLeft, KEY_A},
    //     {GameAction::MoveRight, KEY_D},
    //     {GameAction::MoveUp, KEY_W},
    //     {GameAction::MoveDown, KEY_S},
    // };

    Load();
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
    if (IsKeyPressed(KEY_F1)) showSettings = !showSettings;
    if (!showSettings) return;

    float wheel = GetMouseWheelMove();
    scrollOffset -= wheel * scrollSpeed;
    scrollOffset = std::max(scrollOffset, 0.0f); 
    int totalHeight = 40 + 3 * 40 + 60 + 30 + (int)actionKeyMap.size() * 40;
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

    int y = 40;

    auto handlePlusMinus = [&](int& value, int min, int max, int yPos) {
        Rectangle plusBtn = {290, (float)yPos, 30, 30};
        Rectangle minusBtn = {330, (float)yPos, 30, 30};

        if (CheckCollisionPointRec(mouseVirtual, plusBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            value = std::min(value + 10, max);
        if (CheckCollisionPointRec(mouseVirtual, minusBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            value = std::max(value - 10, min);
    };

    handlePlusMinus(windowWidth, 800, 3840, y); y += 40;
    handlePlusMinus(windowHeight, 600, 2160, y); y += 40;
    handlePlusMinus(maxFPS, 30, 240, y); y += 60;

    y += 30;

    for (const auto& [action, _] : actionKeyMap) {
        Rectangle btn = {200, (float)y, 100, 30};
        Rectangle visibleBtn = btn;
        visibleBtn.y -= scrollOffset;

        if (CheckCollisionPointRec(mouseVirtual, visibleBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            waitingForKey = true;
            currentBinding = action;
        }
        y += 40;
    }

    if (IsKeyPressed(KEY_Q)) {
        Save();
    }
}

void Settings::render() const {
    if (!showSettings) return;

    BeginScissorMode(0, 0, virtualScreenSizeX, virtualScreenSizeY);

    int y = 40 - (int)scrollOffset;
    DrawIntField("Window Width", windowWidth, y); y += 40;
    DrawIntField("Window Height", windowHeight, y); y += 40;
    DrawIntField("Max FPS", maxFPS, y); y += 60;

    DrawText("Keybindings:", 20, y, 20, WHITE); y += 30;

    for (const auto& [action, _] : actionKeyMap) {
        DrawKeyBind(action, y);
        y += 40;
    }

    EndScissorMode();
}

void Settings::DrawIntField(const char* label, int value, int y) const {
    DrawText(label, 20, y, 20, WHITE);
    DrawRectangle(200, y, 80, 30, LIGHTGRAY);
    DrawText(std::to_string(value).c_str(), 210, y + 5, 20, BLACK);

    DrawText("+", 300, y + 5, 20, WHITE);
    DrawText("-", 340, y + 5, 20, WHITE);
}

void Settings::DrawKeyBind(const std::string& action, int y) const {
    DrawText(action.c_str(), 20, y, 20, WHITE);

    Rectangle btn = {200, (float)y, 100, 30};
    DrawRectangleRec(btn, GRAY);

    std::string keyName = (waitingForKey && currentBinding == action)
        ? "..."
        : GetKeyName(actionKeyMap.at(action));

    DrawText(keyName.c_str(), 210, y + 5, 20, WHITE);
}

bool Settings::IsActionPressed(const std::string& action) const {
    auto it = actionKeyMap.find(action);
    return it != actionKeyMap.end() ? IsKeyDown(it->second) : false;
}
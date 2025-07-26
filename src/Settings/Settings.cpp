#include "Settings.h"
#include <fstream>
#include "Utilities/Input/KeysTranslate.h"
#include "Utilities/Logger/Logger.h"
#include "SettingsConstants.h"

Settings::Settings(const std::string& path) : configPath(path) {
    Load();
}

void Settings::Load() {
    widgets.clear();

    std::ifstream file(configPath);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (line == "#screen") {
            widgets.push_back(std::make_unique<SectionHeader>("Screen Settings"));
        } else if (line == "#fps") {
            widgets.push_back(std::make_unique<SectionHeader>("Performance"));
        } else if (line == "#keybinds") {
            widgets.push_back(std::make_unique<SectionHeader>("Key Bindings"));
        } else {
            size_t eq = line.find('=');
            if (eq == std::string::npos) continue;

            std::string key = line.substr(0, eq);
            std::string val = line.substr(eq + 1);

            if (key == "window_width") {
                windowWidth = std::stoi(val);
                widgets.push_back(std::make_unique<IntField>("Window Width", &windowWidth, 640, 9999));
            } else if (key == "window_height") {
                windowHeight = std::stoi(val);
                widgets.push_back(std::make_unique<IntField>("Window Height", &windowHeight, 480, 9999));
            } else if (key == "max_fps") {
                maxFPS = std::stoi(val);
                widgets.push_back(std::make_unique<IntField>("Max FPS", &maxFPS, 30, 1000));
            } else {
                Action action = InputManager::GetActionFromName(key.c_str());
                if (action != Action::COUNT) {
                    int keyCode = GetKeyFromName(val.c_str());
                    InputManager::GetInstance().SetKeyBinding(action, keyCode);
                    widgets.push_back(std::make_unique<KeyBindField>(action));
                }
            }
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
    for (int i = 0; i < static_cast<int>(Action::COUNT); i++) {
        Action action = static_cast<Action>(i);
        file << InputManager::GetActionName(action) << "=" << GetKeyName(InputManager::GetInstance().GetKeyBinding(action)) << "\n";
    }

    SetWindowSize(windowWidth, windowHeight);
    SetTargetFPS(maxFPS);
}

void Settings::Update(float dt, Vector2 mouseVirtual) {
    float wheel = GetMouseWheelMove();
    scrollOffset -= wheel * SCROLL_SPEED;
    scrollOffset = std::max(scrollOffset, 0.0f);

    int totalHeight = INITIAL_Y;
    for (const auto& widget : widgets) {
        totalHeight += widget->GetHeight();
    }

    int maxScroll = std::max(0, totalHeight - virtualScreenSizeY);
    scrollOffset = std::min(scrollOffset, (float)maxScroll);

    int y = INITIAL_Y;
    for (auto& widget : widgets) {
        widget->Update(mouseVirtual, y - scrollOffset);
        y += widget->GetHeight();
    }

    if (IsKeyPressed(KEY_Q)) {
        Save(); // TODO apply button
    }
}

void Settings::Render() const {
    BeginScissorMode(0, 0, virtualScreenSizeX, virtualScreenSizeY);
    int y = INITIAL_Y;
    for (const auto& widget : widgets) {
        widget->Render(y - scrollOffset);
        y += widget->GetHeight();
    }

    EndScissorMode();
}

#pragma once
#include <raylib.h>
#include "Core/Render/Renderer.h"
#include "Constants/GraphicsConst.h"
#include "InputManager.h"
#include "SettingsData.h"
#include "Utilities/Graphics/Button.h"
#include "Widgets.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include "Constants/Path.h"

class Settings {
private:
    std::string configPath = path::settings; // Settings config path
    bool settingsActive = false; // is settings active
    SettingsData data; // numeric data for screen size, fps, etc

    float scrollOffset = 0.0f; // scrolling fucnctional
    // buttons
    Button applyButton; 
    Button quitButton;
    Button defaultButton;

public:
    Settings(const std::string& configPath = path::settings);
    void update(float dt, Vector2 mouseVirtual);
    void render(Renderer& renderer) const;

    // getters and setters
    bool isActive() const { return settingsActive; }
    void setActive(bool active) { settingsActive = active; }
    const SettingsData& getData() const { return data; }

private:
    void init(); // init
    
    // save and load
    void save();
    void load();
    void loadFromFile();
    void apply() const;
    void loadDefault();

    // to easily add new parameters
    using SettingHandler = std::function<void(const std::string&)>;
    std::unordered_map<std::string, SettingHandler> settingHandlers;
    void registerHandlers();

    // headers, int fields, key bind fields
    std::vector<std::unique_ptr<SettingWidget>> widgets;
};

#pragma once
#include <string>
#include <vector>
#include <raylib.h>
#include "Constants/GraphicsConst.h"
#include "InputManager.h"
#include "Widgets.h"
#include <memory>

class Settings {
public:
    Settings(const std::string& configPath = "data/Settings/Settings.cfg");
    void Load();
    void Save();
    void Update(float dt, Vector2 mouseVirtual);
    void Render() const;
    bool isActive() const { return settingsActive; }
    void setActive(bool active) { settingsActive = active; }

private:
    std::string configPath;
    bool settingsActive = false;
    float scrollOffset = 0.0f;
    int windowWidth = virtualScreenSizeX;
    int windowHeight = virtualScreenSizeY;
    int maxFPS = 60;

    std::vector<std::unique_ptr<SettingWidget>> widgets;
};

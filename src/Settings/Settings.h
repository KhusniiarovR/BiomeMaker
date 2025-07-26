#pragma once
#include <string>
#include <map>
#include <raylib.h>
#include "Constants/GraphicsConst.h"
#include <unordered_map>
#include <vector>

class Settings {
public:
    Settings(const std::string& configPath = "data/Settings/Settings.cfg");
    void Load();
    void Save();

    void update(float dt, Vector2 mouseVirtual);
    void render() const;

    bool IsActionPressed(const std::string& action) const;

private:
    std::string configPath;
    float scrollOffset = 0.0f;
    float scrollSpeed = 20.0f;

    int windowWidth = 1280;
    int windowHeight = 720;
    int maxFPS = 60;

    std::vector<std::pair<std::string, int>> actionKeyList;
    std::unordered_map<std::string, int> actionKeyMap; 

    bool showSettings = true;
    bool waitingForKey = false;
    std::string currentBinding;

    void DrawIntField(const char* label, int value, int y) const;
    void DrawKeyBind(const std::string& action, int y) const;

    void ParseLine(const std::string& line);
};

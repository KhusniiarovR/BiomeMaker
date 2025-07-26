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
    ~Settings();
    void Load();
    void Save();

    void update(float dt, Vector2 mouseVirtual);
    void render() const;

    bool IsActionPressed(const std::string& action) const;

private:
    std::string configPath;
    float scrollOffset = 0.0f;

    int windowWidth = virtualScreenSizeX;
    int windowHeight = virtualScreenSizeY;
    int maxFPS = 60;

    std::vector<std::pair<std::string, int>> actionKeyList;
    std::unordered_map<std::string, int> actionKeyMap; 
    std::string editingField = "";
    std::string inputBuffer = ""; 

    bool waitingForKey = false;
    std::string currentBinding;

    void DrawKeyBind(const std::string& action, int y) const;

    void ParseLine(const std::string& line);
    bool IsEditingField(const std::string& name) const;
    void UpdateIntField(const std::string& fieldName, int& value, int min, int max, int y, Vector2 mouseVirtual);
    void RenderIntField(const char* label, const std::string& fieldName, int value, int y) const;
};

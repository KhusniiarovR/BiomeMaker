#pragma once
#include <string>
#include <vector>
#include <raylib.h>
#include "Constants/GraphicsConst.h"
#include "InputManager.h"

class Settings {
public:
    Settings(const std::string& configPath = "data/Settings/Settings.cfg");
    ~Settings();
    
    void Load();
    void Save();
    void Update(float dt, Vector2 mouseVirtual);
    void Render() const;
    bool isActive() const;
    void setActive(bool active);

private:
    bool settingsActive = false;
    struct ConfigSection {
        std::string title;
        int startLine;
    };

    std::string configPath;
    float scrollOffset = 0.0f;
    
    int windowWidth = virtualScreenSizeX;
    int windowHeight = virtualScreenSizeY;
    int maxFPS = 60;
    
    std::vector<ConfigSection> sections;
    std::vector<std::pair<Action, int>> actionKeyList;
    
    std::string editingField;
    std::string inputBuffer;
    
    bool waitingForKey = false;
    Action currentBinding;
    
    void ParseLine(const std::string& line, int lineNumber);
    void DrawSectionTitle(const std::string& title, int& y) const;
    void DrawKeyBind(Action action, int y) const;
    void UpdateIntField(const std::string& fieldName, int& value, int min, int max, int y, Vector2 mouseVirtual);
    void RenderIntField(const char* label, const std::string& fieldName, int value, int y) const;
    bool IsEditingField(const std::string& name) const;
    int CalculateTotalHeight() const;
    int GetSectionContentYPosition(int sectionIndex) const;
    int GetSectionStartYPosition(int sectionIndex) const;
    int GetSectionHeight(int sectionIndex) const;
};
#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include <Core/Render/Renderer.h>
#include "Constants/GraphicsConst.h"
#include "Constants/Path.h"

class WorldSelector { 
private:
    std::string path; // path to all worlds folder
    std::vector<std::string> folders; // list of all worlds
    int selectedIndex = -1; // selected world

    // graphics
    float listAreaHeight = virtualScreenSizeY * 0.70f; // % height of the screen that worldSelector will take
    float worldHeight = listAreaHeight * 0.25f; // height of 1 world
    float worldSpacing = 15.0f; // space between worlds
    Rectangle worldBox = {virtualScreenSizeX * 0.05f, virtualScreenSizeY * 0.2f, virtualScreenSizeX * 0.5f, worldHeight }; // 1 world size
    float textX = worldBox.x + worldBox.width / 2.0f; // text position x

    // scroll
    float scrollOffset = 0.0f; 
    float scrollSpeed = 10.0f;

    // total size of all worlds
    float totalContentHeight;

public:
    explicit WorldSelector(const std::string& path = path::worlds);
    void update(Vector2 mouseVirtual);
    void render(Renderer& renderer) const;

    [[nodiscard]] std::string getSelectedFolder() const;
    void loadFolders();
    void deleteSelected();
};
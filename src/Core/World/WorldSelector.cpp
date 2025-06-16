#include "WorldSelector.h"
#include <filesystem>
#include <algorithm>
#include "Utilities/World/Clamp.h"
#include "Constants/GraphicsConst.h"

WorldSelector::WorldSelector(const std::string& path)
{
    basePath = path;
    listAreaHeight = screenSizeY * 0.70f;
    worldHeight = listAreaHeight * 0.25f;
    worldBox = {screenSizeX * 0.05f, screenSizeY * 0.2f, screenSizeX * 0.5f, worldHeight };
    textX = (worldBox.x + worldBox.width / 2.0f) / screenSizeX;
    scrollSpeed = 10.0f;
    worldSpacing = 15.0f;
    loadFolders();
    totalContentHeight = folders.size() * (worldHeight + worldSpacing);
}
// todo fix buttons incorrect hitbox

void WorldSelector::loadFolders() {
    folders.clear();
    if (!std::filesystem::exists(basePath) || !std::filesystem::is_directory(basePath)) return;

    for (const auto& entry : std::filesystem::directory_iterator(basePath)) {
        if (entry.is_directory()) {
            folders.push_back(entry.path().filename().string());
        }
    }
}

void WorldSelector::update() {
    scrollOffset -= GetMouseWheelMove() * scrollSpeed * 3;
    if (IsKeyDown(KEY_DOWN)) scrollOffset += scrollSpeed;
    if (IsKeyDown(KEY_UP)) scrollOffset -= scrollSpeed;

    scrollOffset = Clamp(scrollOffset, 0.0f, std::max(0.0f, totalContentHeight - listAreaHeight));

    for (size_t i = 0; i < folders.size(); ++i) {
        Rectangle worldBox = this->worldBox;
        worldBox.y = worldBox.y + i * (worldHeight + worldSpacing) - scrollOffset;

        if (worldBox.y + worldHeight < worldBox.y || worldBox.y > worldBox.y + listAreaHeight) {
            continue;
        }

        if (CheckCollisionPointRec(GetMousePosition(), worldBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            selectedIndex = static_cast<int>(i);
        }
    }
}

void WorldSelector::draw(Renderer& renderer) const {
    renderer.drawTextGradient("Choose world: ", {0.5, 0.1}, 100, 3.0f, LIGHTGRAY, DARKGRAY, 0.0f, 0.0f);
    // todo dynamic text size everywhere

    BeginScissorMode(worldBox.x, worldBox.y, worldBox.width, listAreaHeight);
    for (int i = 0; i < folders.size(); ++i) {
        Rectangle worldBox = this->worldBox;
        worldBox.y = worldBox.y + i * (worldHeight + worldSpacing) - scrollOffset;

        if (worldBox.y + worldHeight < worldBox.y || worldBox.y > worldBox.y + listAreaHeight) {
            continue;
        }

        bool hovered = CheckCollisionPointRec(GetMousePosition(), worldBox);
        Color bgColor = GRAY;
        if (hovered) bgColor = LIGHTGRAY;
        if (i == selectedIndex) bgColor = DARKGRAY;

        DrawRectangleRounded(worldBox, 0.15f, 12, bgColor);
        renderer.drawText(folders[i], {textX, (worldBox.y + worldBox.height / 2.0f) / screenSizeY}, 100, BLACK);
    }
    EndScissorMode();
}

std::string WorldSelector::getSelectedFolder() const {
    if (selectedIndex >= 0 && selectedIndex < (int)folders.size()) {
        return basePath + folders[selectedIndex];
    }
    return "";
}

void WorldSelector::deleteCurrent() {
    if (selectedIndex < 0 || selectedIndex >= static_cast<int>(folders.size())) {
        return;
    }
    std::string path = getSelectedFolder();
    if (!path.empty()) {
        std::filesystem::remove_all(path);
        loadFolders();
        totalContentHeight = folders.size() * (worldHeight + worldSpacing);
        scrollOffset = Clamp(scrollOffset, 0.0f, std::max(0.0f, totalContentHeight - listAreaHeight));
        selectedIndex = -1;
    }
}

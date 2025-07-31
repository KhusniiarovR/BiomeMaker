#include "WorldSelector.h"
#include <filesystem>
#include <algorithm>

WorldSelector::WorldSelector(const std::string& path) : path(path)
{  
    loadFolders();
}

void WorldSelector::loadFolders()
{
    folders.clear();
    if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) { return; }

    for (const auto& entry : std::filesystem::directory_iterator(path)) 
    {
        if (entry.is_directory()) { folders.push_back(entry.path().filename().string()); }
    }

    totalContentHeight = folders.size() * (worldHeight + worldSpacing); // recalculate total size
}

void WorldSelector::update(Vector2 mouseVirtual) 
{
    // scroll
    scrollOffset -= GetMouseWheelMove() * scrollSpeed * 3;
    if (IsKeyDown(KEY_DOWN)) { scrollOffset += scrollSpeed; }
    if (IsKeyDown(KEY_UP))   { scrollOffset -= scrollSpeed; }
    scrollOffset = std::clamp(scrollOffset, 0.0f, std::max(0.0f, totalContentHeight - listAreaHeight));

    for (int i = 0; i < folders.size(); ++i) // update every world rect
    {
        Rectangle worldBox = this->worldBox;
        worldBox.y = worldBox.y + i * (worldHeight + worldSpacing) - scrollOffset;

        if (worldBox.y + worldHeight < worldBox.y || worldBox.y > worldBox.y + listAreaHeight) { continue; }

        if (CheckCollisionPointRec(mouseVirtual, worldBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { selectedIndex = i; }
    }
}

void WorldSelector::render(Renderer& renderer) const {
    renderer.drawTextGradient("Choose world: ", {0.5, 0.1}, 20, 4.0f, BLACK, RED, true, true, 0.0f, 0.0f);

    BeginScissorMode(worldBox.x, worldBox.y, worldBox.width, listAreaHeight);
    for (int i = 0; i < folders.size(); ++i) // draw every world rect
    {
        Rectangle worldBox = this->worldBox;
        worldBox.y = worldBox.y + i * (worldHeight + worldSpacing) - scrollOffset;

        if (worldBox.y + worldHeight < worldBox.y || worldBox.y > worldBox.y + listAreaHeight) { continue; }

        bool hovered = CheckCollisionPointRec(renderer.getMouseVirtual(), worldBox);
        Color bgColor = (hovered) ? LIGHTGRAY : GRAY;

        DrawRectangleRounded(worldBox, 0.15f, 12, (i == selectedIndex) ? DARKGRAY : bgColor);
        renderer.drawText(folders[i], {textX, worldBox.y + worldBox.height / 2.0f}, 32, BLACK, true, false);
    }
    EndScissorMode();
}

std::string WorldSelector::getSelectedFolder() const 
{
    if (selectedIndex >= 0 && selectedIndex < (int)folders.size()) { return path + folders[selectedIndex]; }
    return "";
}

void WorldSelector::deleteSelected() 
{
    if (selectedIndex < 0 || selectedIndex >= static_cast<int>(folders.size())) { return; }
    
    std::string pathWorld = getSelectedFolder();
    if (!pathWorld.empty())
    {
        std::filesystem::remove_all(pathWorld);
        loadFolders();
        scrollOffset = std::clamp(scrollOffset, 0.0f, std::max(0.0f, totalContentHeight - listAreaHeight));
        selectedIndex = -1;
    }
}

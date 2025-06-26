#ifndef WORLDSELECTOR_H
#define WORLDSELECTOR_H

#include <raylib.h>
#include <string>
#include <vector>
#include <Core/Render/Renderer.h>

class WorldSelector {
public:
    explicit WorldSelector(const std::string& path = "saves/");
    void update();
    void render(Renderer& renderer) const;
    [[nodiscard]] std::string getSelectedFolder() const;
    void loadFolders();
    void deleteCurrent();

private:
    std::string basePath;
    std::vector<std::string> folders;
    int selectedIndex = -1;

    float listAreaHeight;
    float worldHeight;
    float worldSpacing;
    Rectangle worldBox;
    float textX;
    float scrollOffset = 0.0f;
    float scrollSpeed;
    float totalContentHeight;
};



#endif //WORLDSELECTOR_H

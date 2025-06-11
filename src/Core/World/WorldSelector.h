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
    void draw(Renderer& renderer) const;
    [[nodiscard]] std::string getSelectedFolder() const;

private:
    std::string basePath;
    std::vector<std::string> folders;
    int selectedIndex = -1;

    float listAreaHeight;
    float worldHeight;
    float worldSpacing;
    float listStartX;
    float listStartY;
    Rectangle worldBox;
    float textX;
    float scrollOffset = 0.0f;
    float scrollSpeed;
    float totalContentHeight;

    void LoadFolders();
    void calcY(int i);
};



#endif //WORLDSELECTOR_H

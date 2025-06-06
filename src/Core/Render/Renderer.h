#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include <string>
#include "AssetManager.h"

class Renderer {
private:
    AssetManager& assetManager;
    Camera2D camera;

// todo make camera better

public:
    Renderer(AssetManager& assetManager);
    Camera2D& GetCamera();

    void updateCamera(Vector2 playerPos);

    void drawText(const std::string& text, Vector2 position = {0.5f, 0.5f},
                  float size = 100.0f, Color color = WHITE,
                  const std::string& fontKey = "Inter", float spacing = 4.0f);
    void drawPlayer(Vector2 playerPos);
};



#endif //RENDERER_H

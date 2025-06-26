#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include <string>
#include "AssetManager.h"

class Renderer {
private:
    AssetManager& assetManager;
    Camera2D camera;
    float bgoffset = 0.0f;

// TODO make camera higher than renderer

public:
    Renderer(AssetManager& assetManager);
    Camera2D& GetCamera();

    void updateCamera(Vector2 playerPos);

    void drawText(const std::string& text, Vector2 position = {0.5f, 0.5f},
                  float size = 100.0f, Color color = WHITE,
                  bool isCentered = true, bool isNormalizedPos = true,
                  const std::string& fontKey = "Inter", float spacing = 4.0f); 
                
    void drawTextGradient(
                const std::string& text, Vector2 position = {0.5f, 0.5f},
                float size = 100.0f,  float speed = 4.0f,
                Color colorA = WHITE, Color colorB = BLACK,
                float bounceSpeed = 0.5f, float bounceHeight = 12.0f,
                const std::string& fontKey = "Inter", float spacing = 4.0f);  
                
    void drawBackground();            

    void drawPlayer(Vector2 playerPos);
    void drawEnemy(Vector2 enemyPos);

    Texture2D& getTexture(const std::string& key, bool shouldBeWrapped = false);
};

#endif //RENDERER_H

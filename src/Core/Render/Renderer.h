#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include <string>
#include "AssetManager.h"

// TODO make camera higher than renderer

class Renderer {
private:
    AssetManager& assetManager; // to take assets
    Camera2D camera; // camera 
    
    int virtualWidth; // virtual screen X
    int virtualHeight;// virtual screen Y
    Vector2& mouseVirtual; // mouse for virtual screen

    float bgoffset = 0.0f; // for main menu background

public:
    Renderer(AssetManager& assetManager, int width, int height, Vector2& mouseVirtual);
    
    void updateCameraTarget(Vector2 position);
    Camera2D& GetCamera();

    Vector2 getMouseVirtual();

    // draw text
    void drawText(const std::string& text, Vector2 position = {0.5f, 0.5f},
                  float size = 20.0f, Color color = WHITE,
                  bool isCentered = true, bool isNormalizedPos = true,
                  const std::string& fontKey = "silkscreen", float spacing = 4.0f); 
                
    // draw text with gradient and jumping letter
    void drawTextGradient(
        const std::string& text, Vector2 position = {0.5f, 0.5f},
        float size = 20.0f,  float speed = 4.0f,
        Color colorA = WHITE, Color colorB = BLACK,
        bool isNormalizedPos = true, bool isCentered = true,
        float bounceSpeed = 0.5f, float bounceHeight = 12.0f,
        const std::string& fontKey = "silkscreen", float spacing = 4.0f);  
                
    void drawBackground(); // draw main menu background

    Texture2D& getTexture(const std::string& key, bool shouldBeWrapped = false); // get texture from asset manager
};

#endif //RENDERER_H

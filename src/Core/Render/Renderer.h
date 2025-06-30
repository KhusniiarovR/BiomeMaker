#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include <string>
#include "AssetManager.h"
#include "Items/Inventory/Inventory.h"

class Renderer {
private:
    AssetManager& assetManager;
    Camera2D camera;
    float bgoffset = 0.0f;

    int virtualWidth;
    int virtualHeight;
    Vector2& mouseVirtual;

// TODO make camera higher than renderer

public:
    Renderer(AssetManager& assetManager, int width, int height, Vector2& mouseVirtual);
    Camera2D& GetCamera();

    void updateCamera(Vector2 position);
    Vector2 getMouseVirtual();

    void drawText(const std::string& text, Vector2 position = {0.5f, 0.5f},
                  float size = 20.0f, Color color = WHITE,
                  bool isCentered = true, bool isNormalizedPos = true,
                  const std::string& fontKey = "inter", float spacing = 4.0f); 
                
    void drawTextGradient(
                const std::string& text, Vector2 position = {0.5f, 0.5f},
                float size = 20.0f,  float speed = 4.0f,
                Color colorA = WHITE, Color colorB = BLACK,
                bool isNormalizedPos = true, bool isCentered = true,
                float bounceSpeed = 0.5f, float bounceHeight = 12.0f,
                const std::string& fontKey = "inter", float spacing = 4.0f);  
                
    void drawBackground();       
    
    void drawInventory(const Inventory* inventory);

    Texture2D& getTexture(const std::string& key, bool shouldBeWrapped = false);
};

#endif //RENDERER_H

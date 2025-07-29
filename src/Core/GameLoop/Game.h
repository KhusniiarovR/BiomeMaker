#pragma once
#include "SceneManager.h"
#include "Core/Render/AssetManager.h"
#include "Core/Render/Renderer.h"

class Game {
private:
    AssetManager assetManager; // for loading and unloading textures, sounds and fonts
    Renderer renderer; // to render textures, sounds and fonts
    SceneManager sceneManager; // manage scenes
    Vector2 mouseVirtual; // mouse for virtual screen
    Camera2D camera; // camera

public:
    Game();
    void run();
    
private:
    RenderTexture2D virtualScreen; // virtual screen
    float renderScale = 0.0f;
    int offsetX = 0; // black lines for bad screen resolutions
    int offsetY = 0; // black lines for bad screen resolutions
    
    void init();
    void stop();
    Vector2 getMouseVirtual() const;
};
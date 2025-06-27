#ifndef GAME_H
#define GAME_H

#include "SceneManager.h"
#include "Core/Render/AssetManager.h"
#include "Core/Render/Renderer.h"

class Game {
private:
    AssetManager assetManager;
    Renderer renderer;
    SceneManager sceneManager;
    Vector2 mouseVirtual;

public:
    Game();
    void run();
    
    Vector2 getMouseVirtual() const;

private:
    RenderTexture2D virtualScreen;
    float renderScale = 1.0f;
    int offsetX = 0;
    int offsetY = 0;

    void init();
    void stop();
};



#endif //GAME_H

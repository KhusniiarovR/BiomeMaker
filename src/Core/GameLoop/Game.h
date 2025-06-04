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

public:
    Game();
    void run();

private:
    void init();
    void stop();
};



#endif //GAME_H

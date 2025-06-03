#ifndef GAME_H
#define GAME_H

#include "Core/SceneManager.h"
#include "Core/AssetManager.h"
#include "Core/Renderer.h"

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

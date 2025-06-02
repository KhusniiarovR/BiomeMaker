#ifndef GAME_H
#define GAME_H

#include "Core/SceneManager.h"

class Game {
private:
    SceneManager sceneManager;

public:
    Game();
    void run();

private:
    void init();
    void stop();
};



#endif //GAME_H

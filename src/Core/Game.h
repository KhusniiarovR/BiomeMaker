#ifndef GAME_H
#define GAME_H

#include <memory>
#include "Scenes/Scene.h"
#include "Scenes/SceneType.h"

class Game {
private:
    std::unique_ptr<Scene> currentScene;

public:
    Game();
    ~Game();

    void run();
    void loadScene(SceneType sceneType);

private:
    void init();
    void stop();

    void update(float dt);
    void draw();
};



#endif //GAME_H

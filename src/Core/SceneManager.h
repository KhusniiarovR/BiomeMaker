#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <memory>
#include "Scenes/Scene.h"

class SceneManager {
private:
    std::unique_ptr<Scene> currentScene;

public:
    SceneManager();
    void loadScene(SceneType sceneType);
    void update(float dt);
    void draw();
};

#endif //SCENEMANAGER_H

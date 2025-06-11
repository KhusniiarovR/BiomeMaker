#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <memory>
#include "Scenes/Scene.h"
#include "Core/Render/Renderer.h"

class SceneManager {
private:
    std::unique_ptr<Scene> currentScene;
    Renderer& renderer;

public:
    SceneManager(Renderer& renderer);
    void loadScene(SceneType sceneType, const std::string& worldName = "\0");
    void update(float dt);
    void render();
};

#endif //SCENEMANAGER_H

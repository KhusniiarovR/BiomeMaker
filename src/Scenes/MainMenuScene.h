#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "Scene.h"
#include "Core/World/WorldCreator.h"

class MainMenuScene : public Scene {
private:
    bool changeScene = false;
    WorldCreator worldCreator;

public:
    MainMenuScene(Renderer& renderer);
    void update(float dt) override;
    void render() const override;
    [[nodiscard]] bool shouldTransition() const override;
    [[nodiscard]] SceneType getNextScene() const override;
};

#endif //MAINMENUSCENE_H

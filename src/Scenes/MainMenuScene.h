#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "Scene.h"

class MainMenuScene : public Scene {
public:
    MainMenuScene(Renderer& renderer);
    void update(float dt) override;
    void render() const override;
    [[nodiscard]] bool shouldTransition() const override;
    [[nodiscard]] SceneType getNextScene() const override;
};

#endif //MAINMENUSCENE_H

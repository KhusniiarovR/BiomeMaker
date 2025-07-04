#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "Scene.h"

class MainMenuScene : public Scene {
public:
    MainMenuScene(Renderer& renderer);

    void update(float dt, Vector2 mouseVirtual) override;
    void render() const override;

    void updateChangeScene() override;
    [[nodiscard]] bool shouldTransition() const override;
    [[nodiscard]] SceneType getNextScene() const override;
};

#endif //MAINMENUSCENE_H

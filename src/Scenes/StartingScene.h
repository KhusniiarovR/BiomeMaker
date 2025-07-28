#pragma once
#include "Scene.h"

class StartingScene : public Scene { // first scene that only say hello and wait for input
public:
    StartingScene(Renderer& renderer);

    void update(float dt, Vector2 mouseVirtual) override;
    void render() const override;

    void updateChangeScene() override;
    [[nodiscard]] bool shouldTransition() const override;
    [[nodiscard]] SceneType getNextScene() const override;
};

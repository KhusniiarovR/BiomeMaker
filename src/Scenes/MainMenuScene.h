#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "Scene.h"

class MainMenuScene : public Scene {
private:
    bool changeScene = false;

public:
    void update(float dt) override;
    void draw() const override;
    [[nodiscard]] bool shouldTransition() const override;
    [[nodiscard]] SceneType getNextScene() const override;
};

#endif //MAINMENUSCENE_H

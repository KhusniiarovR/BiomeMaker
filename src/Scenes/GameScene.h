#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include <Entities/Player.h>

class GameScene : public Scene {
private:
    bool changeScene = false;
    Player player;

public:
    GameScene(Renderer& renderer);
    void update(float dt) override;
    void render() const override;
    [[nodiscard]] bool shouldTransition() const override;
    [[nodiscard]] SceneType getNextScene() const override;
};



#endif //GAMESCENE_H

#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <Constants/GraphicsConst.h>
#include "Scene.h"
#include <Entities/Player.h>
#include "Core/World/World.h"

class GameScene : public Scene {
private:
    bool changeScene = false;
    Player player;
    World world;

public:
    GameScene(Renderer& renderer);
    void update(float dt) override;
    void render() const override;
    [[nodiscard]] bool shouldTransition() const override;
    [[nodiscard]] SceneType getNextScene() const override;
};



#endif //GAMESCENE_H

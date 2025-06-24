#ifndef GAMESCENE_H
#define GAMESCENE_H


#include "Scene.h"
#include <Entities/Player.h>
#include <Entities/Enemy.h>
#include "World/World.h"

class GameScene : public Scene {
private:
    Enemy enemy;
    Player player;
    World world;

public:
    GameScene(Renderer& renderer, const std::string& worldName);
    void update(float dt) override;
    void render() const override;
    [[nodiscard]] bool shouldTransition() const override;
    [[nodiscard]] SceneType getNextScene() const override;
};



#endif //GAMESCENE_H

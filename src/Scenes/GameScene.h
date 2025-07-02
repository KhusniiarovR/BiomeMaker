#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "Entities/Player.h"
#include "Entities/Enemy.h"
#include "World/World.h"

class GameScene : public Scene {
private:
    Enemy enemy;
    Player player;
    World world;

public:
    GameScene(Renderer& renderer, const std::string& worldName);

    void update(float dt, Vector2 mouseVirtual) override;
    void render() const override;

    void updateChangeScene() override;
    [[nodiscard]] bool shouldTransition() const override;
    [[nodiscard]] SceneType getNextScene() const override;

private:
    void updatePlayer (float dt, Vector2 mouseVirtual);
    void updateEnemies(float dt);
    void updateObjects(Vector2 mouseVirtual);
    void updateWorld  (Vector2 mouseVirtual);
    void updateCamera ();
};

#endif //GAMESCENE_H

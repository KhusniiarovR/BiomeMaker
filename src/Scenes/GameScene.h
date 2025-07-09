#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "Entities/Player/Player.h"
#include "Entities/Enemy/EnemyManager.h"
#include "World/World.h"

class GameScene : public Scene {
private:
    World world;
    CollisionWorld collision;
    Player player;
    EnemyManager enemies;

public:
    GameScene(Renderer& renderer, const std::string& worldName);
    ~GameScene() override;

    void update(float dt, Vector2 mouseVirtual) override;
    void render() const override;

    void updateChangeScene() override;
    [[nodiscard]] bool shouldTransition() const override;
    [[nodiscard]] SceneType getNextScene() const override;

private:
    void updatePlayer (float dt, Vector2 mouseVirtual);
    void updateEnemies(float dt);
    void updateWorld  (Vector2 mouseVirtual);
    void updateCamera ();
};

#endif //GAMESCENE_H

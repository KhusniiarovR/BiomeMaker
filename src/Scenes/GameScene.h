#pragma once
#include "Scene.h"
#include "Entities/Player/Player.h"
#include "Entities/Enemy/EnemyManager.h"
#include "World/World.h"
#include "Ui/GameUi.h"
#include "GameScenePause.h"

class GameScene : public Scene {
private:
    World world;             // world (chunks(tiles, objects))
    CollisionWorld collision;// easy access to world objects

    Player player;           // player and inventory
    GameUi ui;               // shows user interface (must be drawn last)

    EnemyManager enemies;    // vector with enemies

    bool paused = false;     // pause
    PauseMenu pauseMenu;     // pause

public:
    GameScene(Renderer& renderer, const std::string& worldName);
    ~GameScene() override;

    void update(float dt, Vector2 mouseVirtual) override;
    void render() const override;

    void updateChangeScene() override;
    [[nodiscard]] bool shouldTransition() const override;
    [[nodiscard]] SceneType getNextScene() const override;

private:
    // separate update into several functions
    void updatePause  (Vector2 mouseVirtual); 
    void updatePlayer (float dt, Vector2 mouseVirtual);
    void updateEnemies(float dt);
    void updateWorld  (Vector2 mouseVirtual);
    void updateCamera ();
};
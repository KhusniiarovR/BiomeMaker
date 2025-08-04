#pragma once
#include "Scene.h"
#include "World/World.h"
#include "Entities/Player/Player.h"
#include "Entities/Enemy/EnemyManager.h"
#include "Entities/Collisions/Collision.h"
#include "Entities/Collisions/GameCollisionProvider.h"
#include "Ui/GameUi.h"
#include "GameScenePause.h"

class GameScene : public Scene { // Game
private:
    // world
    World world;             // (chunks(tiles, objects))

    // collisions
    Collision collision;
    GameCollisionProvider provider;

    // player
    Player player;           // player and inventory
    GameUi ui;               // shows user interface (must be drawn last)

    // vector with enemies
    EnemyManager enemies;

    // pause
    bool paused = false;
    PauseMenu pauseMenu;

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
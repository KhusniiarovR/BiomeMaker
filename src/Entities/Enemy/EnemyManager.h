#pragma once
#include "Entities/Enemy/Enemy.h"
#include "Core/Render/Renderer.h"
#include "Entities/Collisions/CollisionWorld.h"
#include "World/World.h"
#include <vector>

class EnemyManager { // container for every enemy
private:
    std::vector<Enemy> enemies; // enemy vector

    /* spawn variables */
    float spawnCooldown = 0.0f;
    const float maxCooldown = 3.0f;
    float spawnDistance = 400.0f;
    const int randomOffset = 200;
    int maxEnemies = 10;
    // TODO better spawn logic

    /* others */
    Player& player;
    const CollisionBase* collision; // collision for enemy

public:
    EnemyManager(Player& player, const CollisionBase* collision);
    void update(float dt);
    void render(Renderer& renderer) const;

    /* getters and setters */
    const std::vector<Enemy>& getEnemies() const { return enemies; }
    int getEnemiesSize() const { return enemies.size(); }
    
private:
    void spawn();
};
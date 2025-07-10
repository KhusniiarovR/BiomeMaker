#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include "Entities/Enemy/Enemy.h"
#include "Core/Render/Renderer.h"
#include "Entities/Collisions/CollisionWorld.h"
#include "World/World.h"
#include <vector>

class EnemyManager {
private:
    std::vector<Enemy> enemies;
    float spawnCooldown = 0.0f;
    const float maxCooldown = 3.0f;
    float spawnDistance = 400.0f;
    const int randomOffset = 200;
    int maxEnemies = 10;

    Player& player;
    const CollisionBase* collision;

public:
    EnemyManager(Player& player, const CollisionBase* collision);
    void update(float dt);
    void render(Renderer& renderer) const;

    void spawn();
    const std::vector<Enemy>& getEnemies() const { return enemies; }
    int getEnemiesSize() const { return enemies.size(); }
};

#endif // ENEMYMANAGER_H
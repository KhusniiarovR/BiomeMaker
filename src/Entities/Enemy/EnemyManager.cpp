#include "EnemyManager.h"
#include "Utilities/Logger/Logger.h"

EnemyManager::EnemyManager(Player& player, const CollisionBase* collision)
: player(player), collision(collision) {}

void EnemyManager::update(float dt) {
    spawnCooldown -= dt;
    
    if (spawnCooldown <= 0.0f && enemies.size() < maxEnemies) {
        spawn();
        spawnCooldown = maxCooldown;
    }

    for (auto& enemy : enemies) {
        enemy.update(dt);
    }
}

void EnemyManager::render(Renderer& renderer) const {
    for (const auto& enemy : enemies) {
        enemy.render(renderer);
    }
}

void EnemyManager::spawn() {
    Vector2 playerPos = player.getPosition();
    Vector2 spawnPos;

    int side = GetRandomValue(0, 3);
    switch (side) {
        case 0:
            spawnPos.x = playerPos.x + GetRandomValue(-randomOffset, randomOffset);
            spawnPos.y = playerPos.y - spawnDistance;
        break;
        case 1:
            spawnPos.x = playerPos.x + GetRandomValue(-randomOffset, randomOffset);
            spawnPos.y = playerPos.y + spawnDistance;
        break;
        case 2:
            spawnPos.x = playerPos.x - spawnDistance;
            spawnPos.y = playerPos.y + GetRandomValue(-randomOffset, randomOffset);
        break;
        case 3:
            spawnPos.x = playerPos.x + spawnDistance;
            spawnPos.y = playerPos.y + GetRandomValue(-randomOffset, randomOffset);
        break;
    }

    enemies.emplace_back(spawnPos, player, collision);
    spawnCooldown = maxCooldown;
}
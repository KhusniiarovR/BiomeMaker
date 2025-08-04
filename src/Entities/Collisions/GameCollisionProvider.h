#pragma once
#include "ICollisionDataProvider.h"

class World;
class Player;
class EnemyManager;

class GameCollisionProvider : public ICollisionDataProvider { // to keep collision class easier to read
private:
    const World* world = nullptr;
    const Player* player = nullptr;
    const EnemyManager* enemies = nullptr;

public:
    // set pointers to use collisions
    void setWorld(const World* w) { world = w; }
    void setPlayer(const Player* p) { player = p; }
    void setEnemies(const EnemyManager* e) { enemies = e; }

    Rectangle getPlayerHitbox() const override;
    std::vector<Rectangle> getEnemyHitboxes() const override;
    std::vector<Rectangle> getWorldObjectHitboxes() const override;
};

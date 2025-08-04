#include "GameCollisionProvider.h"
#include "World/World.h"
#include "Entities/Player/Player.h"
#include "Entities/Enemy/EnemyManager.h"
#include "Constants/GraphicsConst.h"

Rectangle GameCollisionProvider::getPlayerHitbox() const 
{
    return player->getBoundingBox();
}

std::vector<Rectangle> GameCollisionProvider::getEnemyHitboxes() const 
{
    std::vector<Rectangle> boxes;
    for (const auto& e : enemies->getEnemies()) { boxes.push_back(e.getBoundingBox()); }
    return boxes;
}

std::vector<Rectangle> GameCollisionProvider::getWorldObjectHitboxes() const 
{
    std::vector<Rectangle> boxes;
    for (const auto& obj : world->getObjectsAll()) 
    { 
        if (obj.hasCollision()) { boxes.push_back(obj.getHitbox(worldTileSize)); }
    }
    return boxes;
}

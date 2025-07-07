#pragma once

#include "raylib.h"
#include "Entities/Entity.h"
#include "Entities/Player/Player.h"
#include "Entities/Collisions/CollisionBase.h"
#include <string>
class Enemy : public Entity
{
private:
    Player &player;
    const CollisionBase* collision;
    float speed = 40.0f;
    int index = 0;
    const int tilemapOffset = 4;
    
public:
    Enemy(Vector2 initPos, Player &player, const CollisionBase* collision);
    void update(float dt) override;
    void render(Renderer& renderer) const;
    
    [[nodiscard]] Vector2 getPosition() const;
    Rectangle getBoundingBox() const override;
    void tryMove(float dx, float dy);
};

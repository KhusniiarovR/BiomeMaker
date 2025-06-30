#pragma once

#include "raylib.h"
#include "Entity.h"
#include "Player.h"
#include "string"
class Enemy : public Entity
{
private:
    float speed;
    Player &player;
public:
    Enemy(Vector2 init_pos, Player &player);
    void update(float dt) override;
    void draw(Renderer& renderer) const;
    
    int index = 0;
    const int tilemapOffset = 4;
    
    [[nodiscard]] Vector2 getPosition() const;
};

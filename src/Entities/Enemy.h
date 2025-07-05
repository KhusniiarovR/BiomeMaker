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
    int index = 0;
    const int tilemapOffset = 4;
    std::function<bool(Rectangle)> collisionCallback;

public:
    Enemy(Vector2 init_pos, Player &player);
    void update(float dt) override;
    void render(Renderer& renderer) const;
    
    [[nodiscard]] Vector2 getPosition() const;
    Rectangle getBoundingBox() const override;
    void setCollisionCallback(std::function<bool(Rectangle)> func) { collisionCallback = std::move(func); }
    void tryMove(float dx, float dy);
};

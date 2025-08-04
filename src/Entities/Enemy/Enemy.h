#pragma once
#include "raylib.h"
#include "Entities/Entity.h"
#include "Entities/Player/Player.h"
#include "Entities/Collisions/Collision.h"
#include "Animations/Animation.h"
#include <string>

class Enemy : public Entity {
private:
    Player &player;
    const Collision* collision; 
    float speed = 40.0f, speedMultiplier = 1.0f; // speed
    Animation animation; // animation
    float attackCooldown = 1.0f; float attackTimer = 0.0f; // attack
    
public:
    Enemy(Vector2 initPos, Player &player, const Collision* collision);
    void update(float dt) override;
    void render(Renderer& renderer) const;
    
    // getters and setters
    [[nodiscard]] Vector2 getPosition() const;
    Rectangle getBoundingBox() const override;
    
private:
    bool tryMove(float dx, float dy);
};

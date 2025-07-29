#pragma once
#include "raylib.h"
#include "Entities/Entity.h"
#include "Entities/Player/Player.h"
#include "Entities/Collisions/CollisionBase.h"
#include "Animations/Animation.h"
#include <string>

class Enemy : public Entity {
private:
    Player &player;
    const CollisionBase* collision; 
    float speed = 40.0f, speedMultiplier = 1.0f; // speed
    Animation animation; // animation
    
public:
    Enemy(Vector2 initPos, Player &player, const CollisionBase* collision);
    void update(float dt) override;
    void render(Renderer& renderer) const;
    
    // getters and setters
    [[nodiscard]] Vector2 getPosition() const;
    Rectangle getBoundingBox() const override;
    
private:
    bool tryMove(float dx, float dy);
};

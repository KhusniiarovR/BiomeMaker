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
        
        float rotation = 0.0f;
        Vector2 direction;
        std::string currentDir = "Right";
        [[nodiscard]] float getPositionX() const;
        [[nodiscard]] float getPositionY() const;
        [[nodiscard]] Vector2 getPosition() const;
};

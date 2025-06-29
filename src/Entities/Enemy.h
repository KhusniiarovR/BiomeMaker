#pragma once

#include "raylib.h"
#include "Entity.h"
#include "Player.h"

class Enemy : public Entity
{
    private:
        float speed;
        Player &player;
    public:
        Enemy(Vector2 init_pos, Player &player);
        void update(float dt) override;
        Texture2D texture;  
        float rotation;
        [[nodiscard]] float getPositionX() const;
        [[nodiscard]] float getPositionY() const;
        [[nodiscard]] Vector2 getPosition() const;
};

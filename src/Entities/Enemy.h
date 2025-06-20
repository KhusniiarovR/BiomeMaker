#pragma once

#include "raylib.h"
#include "Entity.h"
#include "Player.h"

class Enemy : public Entity
{
    private:
        float speed;
    public:
        Enemy(Vector2 init_pos);
        void update(float dt) override;
        void move(Player &player);
        [[nodiscard]] float getPositionX() const;
        [[nodiscard]] float getPositionY() const;
        [[nodiscard]] Vector2 getPosition() const;
};

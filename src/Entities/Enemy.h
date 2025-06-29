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
        Texture2D textureUp;  
        Texture2D textureRight;
        Texture2D textureLeft;  
        Texture2D textureDown;
        
        float rotation;
        Vector2 direction;
        std::string currentDir;
        [[nodiscard]] float getPositionX() const;
        [[nodiscard]] float getPositionY() const;
        [[nodiscard]] Vector2 getPosition() const;
};

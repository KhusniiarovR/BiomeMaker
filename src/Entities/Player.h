#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "Entity.h"

class Player : public Entity {
private:
    float speed;

public:
    Player(Vector2 init_pos);
    void update(float dt) override;
    Texture2D texture;  
    float rotation;
    [[nodiscard]] float getPositionX() const;
    [[nodiscard]] float getPositionY() const;
    [[nodiscard]] Vector2 getPosition() const;
};

#endif //PLAYER_H

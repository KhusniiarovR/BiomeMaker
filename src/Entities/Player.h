#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "Entity.h"
#include <string>
class Player : public Entity {
private:
    float speed;

public:
    Player(Vector2 init_pos);
    void update(float dt) override;
    Texture2D textureDown;  
    Texture2D textureRight;
    Texture2D textureLeft;  
    Texture2D textureUp;
    bool movingUp;
    bool movingDown;
    bool movingLeft;
    bool movingRight;
    std::string lastDirection;
    float rotation;
    [[nodiscard]] float getPositionX() const;
    [[nodiscard]] float getPositionY() const;
    [[nodiscard]] Vector2 getPosition() const;
};

#endif //PLAYER_H

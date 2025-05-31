#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "Entity.h"

class Player : public Entity {
private:
    float speed;
    float radius;

public:
    Player(Vector2 init_pos);
    void update(float dt) override;
    void draw() const override;
};

#endif //PLAYER_H

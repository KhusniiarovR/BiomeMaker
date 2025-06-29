#include "Player.h"

Player::Player(Vector2 init_pos) : Entity(init_pos){
    speed = 80.0f;
    textureDown = LoadTexture("playerDown.png");
    textureLeft = LoadTexture("playerLeft.png");
    textureRight = LoadTexture("playerRight.png");
    textureUp = LoadTexture("playerUp.png");
    rotation = 0.0f;
}

void Player::update(float dt) {
    movingUp = false;
    movingDown = false;
    movingLeft = false;
    movingRight = false;
    if (IsKeyDown(KEY_W)) {position.y -= speed * dt; movingUp = true;}
    if (IsKeyDown(KEY_S)) {position.y += speed * dt; movingDown = true;}
    if (IsKeyDown(KEY_A)) {position.x -= speed * dt; movingLeft = true;}
    if (IsKeyDown(KEY_D)) {position.x += speed * dt; movingRight = true;}
}

float Player::getPositionX() const {
    return position.x;
}

float Player::getPositionY() const {
    return position.y;
}

Vector2 Player::getPosition() const {
    return position;
}
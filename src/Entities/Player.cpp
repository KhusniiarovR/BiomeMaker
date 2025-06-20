#include "Player.h"
#include <iostream>

Player::Player(Vector2 init_pos) {
    position = init_pos;
    speed = 2000.0f; // debug
}

void Player::update(float dt) {
    if (IsKeyDown(KEY_W)) position.y -= speed * dt;
    if (IsKeyDown(KEY_S)) position.y += speed * dt;
    if (IsKeyDown(KEY_A)) position.x -= speed * dt;
    if (IsKeyDown(KEY_D)) position.x += speed * dt;
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
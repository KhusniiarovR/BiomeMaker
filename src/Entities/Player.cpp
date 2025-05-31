#include "Player.h"

Player::Player(Vector2 init_pos) {
    position = init_pos;
    speed = 200.0f;
    radius = 20.0f;
}

void Player::update(float dt) {
    if (IsKeyDown(KEY_W)) position.y -= speed * dt;
    if (IsKeyDown(KEY_S)) position.y += speed * dt;
    if (IsKeyDown(KEY_A)) position.x -= speed * dt;
    if (IsKeyDown(KEY_D)) position.x += speed * dt;
}

void Player::draw() const {
    DrawCircleV(position, radius, GREEN);
}

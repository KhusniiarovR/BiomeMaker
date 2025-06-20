#include "Enemy.h"
#include "Player.h"
#include <cmath>
#include <iostream>
Enemy::Enemy(Vector2 init_pos) {
    position = init_pos;
    position.x -=200;
    speed = 1.5f;
}
void Enemy::update(float dt) {
}
void Enemy::move(Player &player){
    if (position.x < player.position.x)
        position.x += speed;
    else if (position.x > player.position.x)
        position.x -= speed;
    if (position.y < player.position.y)
        position.y += speed;
    else if (position.y > player.position.y)
        position.y -= speed;
}
float Enemy::getPositionX() const {
    return position.x;
}

float Enemy::getPositionY() const {
    return position.y;
}

Vector2 Enemy::getPosition() const {
    return position;
}
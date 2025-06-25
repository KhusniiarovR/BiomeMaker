#include "Enemy.h"
#include "EnemyManager.h"
Enemy::Enemy(Vector2 init_pos,Player &player) : Entity(init_pos), player(player){
    position.x -=200;
    speed = 1.5f;
}

void Enemy::update(float dt) {
    moveToPlayer(player, position, speed);
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
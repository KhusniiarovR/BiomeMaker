#include "Enemy.h"
#include "EnemyManager.h"
Enemy::Enemy(Vector2 init_pos,Player &player) : Entity(init_pos), player(player){
    position.x -= 200;
    speed = 0.5f;
    textureUp = LoadTexture("enemyUp.png");
    textureLeft = LoadTexture("enemyLeft.png");
    textureRight = LoadTexture("enemyRight.png");
    textureDown = LoadTexture("enemyDown.png");
    rotation = 0.0f;
}

void Enemy::update(float dt) {
    moveToPlayer(player, position, speed, currentDir);
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
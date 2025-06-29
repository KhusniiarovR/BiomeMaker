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
    Vector2 diff = {
    player.position.x - position.x,
    player.position.y - position.y
};
    float length = sqrtf(diff.x * diff.x + diff.y * diff.y);
    direction = { diff.x / length, diff.y / length };
}

void Enemy::update(float dt) {
    moveToPlayer(player, position, speed);

    Vector2 toPlayer = {
    player.position.x - position.x,
    player.position.y - position.y
};

float len = sqrtf(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
if (len != 0.0f)
{
    direction = { toPlayer.x / len, toPlayer.y / len };
    position.x += direction.x * speed * dt;
    position.y += direction.y * speed * dt;
}

float absX = fabsf(direction.x);
float absY = fabsf(direction.y);

if (absX > absY) {
    if (direction.x > 0)
        currentDir = "Right";
    else
        currentDir = "Left";
} 
else {
    if (direction.y > 0)
        currentDir = "Down";
    else
        currentDir = "Up";
}

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
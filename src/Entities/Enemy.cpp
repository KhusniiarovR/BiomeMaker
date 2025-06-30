#include "Enemy.h"
#include "EnemyManager.h"
Enemy::Enemy(Vector2 init_pos,Player &player) : Entity(init_pos), player(player){
    position.x -= 200;
    speed = 0.5f;
    rotation = 0.0f;
}

void Enemy::update(float dt) {
    moveToPlayer(player, position, speed, currentDir);
}

void Enemy::draw(Renderer& renderer) const {
    Texture2D enemyTexture = renderer.getTexture("enemy" + currentDir, true);
    Rectangle source = { 0, 0, (float)enemyTexture.width, (float)enemyTexture.height };
    Rectangle dest = { position.x, position.y, (float)enemyTexture.width, (float)enemyTexture.height };
    Vector2 origin = { enemyTexture.width / 2.0f, enemyTexture.height / 2.0f };

    DrawTexturePro(enemyTexture, source, dest, origin, rotation, WHITE);
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
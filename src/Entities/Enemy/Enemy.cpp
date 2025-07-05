#include "Enemy.h"
#include "Constants/TilemapConst.h"
#include "Utilities/Logger/Logger.h"

Enemy::Enemy(Vector2 init_pos,Player &player) : Entity(init_pos), player(player)
{
    position.x -= 200;
    speed = 40.0f;
}

void Enemy::update(float dt) {
    Vector2 toPlayer = {
        player.position.x - position.x,
        player.position.y - position.y
    };

    float dist = sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
    if (dist < 1e-6f) return;

    float moveSpeed = speed * dt;
    float dx = toPlayer.x / dist * moveSpeed;
    float dy = toPlayer.y / dist * moveSpeed;

    tryMove(dx, dy);

    float absX = fabsf(dx);
    float absY = fabsf(dy);
    if (absX > absY) {
        index = (dx > 0) ? 3 : 2;
    } 
    else {
        index = (dy > 0) ? 1 : 0;
    }
}


void Enemy::render(Renderer& renderer) const {
    Texture2D& enemyTexture = renderer.getTexture("entityTilemap");

    int indexX = (index + tilemapOffset) % entityTilesPerRow;
    int indexY = (index + tilemapOffset) / entityTilesPerRow;

    Rectangle source = {
        (float)(indexX * entityTileSize),
        (float)(indexY * entityTileSize),
        (float)entityTileSize,
        (float)entityTileSize
    };

    Rectangle dest = { position.x, position.y, entityTileSize , entityTileSize};
    Vector2 origin = { entityTileSize / 2.0f, entityTileSize / 2.0f };
    DrawTexturePro(enemyTexture, source, dest, origin, 0.0f, WHITE);
}

Vector2 Enemy::getPosition() const {
    return position;
}

Rectangle Enemy::getBoundingBox() const {
    float width = 0.45f * entityTileSize;
    float height = 0.9f * entityTileSize;
    return {
        position.x - width / 2.0f,
        position.y - height / 2.0f,
        width,
        height
    };
}

void Enemy::tryMove(float dx, float dy) {
    Rectangle oldBox = getBoundingBox();

    Rectangle newBox = oldBox;
    newBox.x += dx;
    newBox.y += dy;

    if (!collisionCallback || !collisionCallback(newBox)) {
        position.x += dx;
        position.y += dy;
    }
    else {
        newBox.x = oldBox.x + dx;
        newBox.y = oldBox.y;
        if (!collisionCallback || !collisionCallback(newBox)) {
            position.x += dx;
        }

        newBox.x = oldBox.x;
        newBox.y = oldBox.y + dy;
        if (!collisionCallback || !collisionCallback(newBox)) {
            position.y += dy;
        }
    }
}
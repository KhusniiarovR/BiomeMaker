#include "Enemy.h"
#include "EnemyManager.h"
#include "Constants/TilemapConst.h"
#include "Utilities/Logger/Logger.h"

Enemy::Enemy(Vector2 init_pos,Player &player) : Entity(init_pos), player(player)
{
    position.x -= 200;
    speed = 0.5f;
}

void Enemy::update(float dt) {
    moveToPlayer(player, position, speed, index);
    index += tilemapOffset;
}

void Enemy::draw(Renderer& renderer) const {
    Texture2D enemyTexture = renderer.getTexture("entityTilemap");

    int indexX = index % entityTilesPerRow;
    int indexY = index / entityTilesPerRow;

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
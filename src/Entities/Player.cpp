#include "Player.h"
#include "Constants/WorldConst.h"
#include "Constants/GraphicsConst.h"
#include "Constants/TilemapConst.h"

Player::Player(Vector2 init_pos) 
      : Entity(init_pos),
        hp{{0.01f, 0.05f}, {0.15f, 0.05f}, RED, GRAY, "HEALTH", 10},
        buffSystem(*this)
{
    speed = 80.0f;
    hp.setProgress(1.0f);
}

void Player::update(float dt) {
    float moveSpeed = speed * dt * speedMultiplier;
    float dx = 0, dy = 0;

    if (IsKeyDown(KEY_A)) { dx -= moveSpeed; index = 2; }
    if (IsKeyDown(KEY_D)) { dx += moveSpeed; index = 3; }
    if (IsKeyDown(KEY_W)) { dy -= moveSpeed; index = 0; }
    if (IsKeyDown(KEY_S)) { dy += moveSpeed; index = 1; }

    tryMove(dx, dy);

    if (IsKeyDown(KEY_Q)) { hp.decrease(0.05f); }

    buffSystem.update(dt);
}

void Player::render(Renderer& renderer) const {
    Texture2D& playerTexture = renderer.getTexture("entityTilemap");
    
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
    DrawTexturePro(playerTexture, source, dest, origin, 0.0f, WHITE);

    DrawCircleLinesV(position, handDistance * worldTileSize, YELLOW);
    EndMode2D();
    is2DModeDone = true;
    hp.render(renderer);
}

Vector2 Player::getPosition() const {
    return position;
}

void Player::giveItem(ItemID id, uint8_t count) {
    inventory.addItem(id, count);
}

void Player::heal(float value) {
    hp.increase(value);
}

bool Player::applyEffect(const BuffEffect& effect) {
    buffSystem.addBuff(effect);
    return true;
}

Rectangle Player::getBoundingBox() const {
    float width = 0.45f * entityTileSize;
    float height = 0.9f * entityTileSize;
    return {
        position.x - width / 2.0f,
        position.y - height / 2.0f,
        width,
        height
    };
}

void Player::tryMove(float dx, float dy) {
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
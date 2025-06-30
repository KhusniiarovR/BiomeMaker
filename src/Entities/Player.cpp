#include "Player.h"
#include "Constants/WorldConst.h"
#include "Constants/GraphicsConst.h"
#include "Constants/TilemapConst.h"

Player::Player(Vector2 init_pos) : Entity(init_pos){
    speed = 80.0f;
}

void Player::update(float dt) {
    if (IsKeyDown(KEY_W)) {position.y -= speed * dt; index = 0;}
    if (IsKeyDown(KEY_S)) {position.y += speed * dt; index = 1;}
    if (IsKeyDown(KEY_A)) {position.x -= speed * dt; index = 2;}
    if (IsKeyDown(KEY_D)) {position.x += speed * dt; index = 3;}
}

void Player::render(Renderer& renderer) const {
    Texture2D playerTexture = renderer.getTexture("entityTilemap");
    
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
    renderer.drawInventory(&inventory);
}

Vector2 Player::getPosition() const {
    return position;
}

void Player::giveItem(ItemID id, uint8_t count) {
    inventory.addItem(id, count);
}
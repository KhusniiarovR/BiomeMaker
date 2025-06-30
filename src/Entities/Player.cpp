#include "Player.h"
#include "Constants/WorldConst.h"
#include "Constants/GraphicsConst.h"

Player::Player(Vector2 init_pos) : Entity(init_pos){
    speed = 80.0f;
    rotation = 0.0f;
    lastDirection = "playerDown";
}

void Player::update(float dt) {
    movingUp = false;
    movingDown = false;
    movingLeft = false;
    movingRight = false;
    if (IsKeyDown(KEY_W)) {position.y -= speed * dt; movingUp = true;}
    if (IsKeyDown(KEY_S)) {position.y += speed * dt; movingDown = true;}
    if (IsKeyDown(KEY_A)) {position.x -= speed * dt; movingLeft = true;}
    if (IsKeyDown(KEY_D)) {position.x += speed * dt; movingRight = true;}

    if (movingUp)
        lastDirection = "playerUp";
    else if (movingDown)
        lastDirection = "playerDown";
    else if (movingLeft)
        lastDirection = "playerLeft";
    else if (movingRight)
        lastDirection = "playerRight";
}

void Player::render(Renderer& renderer) const {
    Texture2D playerTexture = renderer.getTexture(lastDirection, true);
    
    Rectangle source = { 0, 0, (float)playerTexture.width, (float)playerTexture.height };
    Rectangle dest = { position.x, position.y, (float)playerTexture.width, (float)playerTexture.height };
    Vector2 origin = { playerTexture.width / 2.0f, playerTexture.height / 2.0f };
    DrawTexturePro(playerTexture, source, dest, origin, rotation, WHITE);

    DrawCircleLinesV(position, handDistance * tileSize, YELLOW);
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
#include "Player.h"
#include "Constants/WorldConst.h"
#include "Constants/GraphicsConst.h"

Player::Player(Vector2 init_pos) : Entity(init_pos){
    speed = 80.0f;
}

void Player::update(float dt) {
    if (IsKeyDown(KEY_W)) position.y -= speed * dt;
    if (IsKeyDown(KEY_S)) position.y += speed * dt;
    if (IsKeyDown(KEY_A)) position.x -= speed * dt;
    if (IsKeyDown(KEY_D)) position.x += speed * dt;
}

void Player::render(Renderer& renderer) const {
    DrawCircleV(position, 10, RED);
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
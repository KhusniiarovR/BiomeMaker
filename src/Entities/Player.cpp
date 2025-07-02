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
    if (IsKeyDown(KEY_A)) {position.x -= speed * dt * speedMultiplier; index = 2;}
    if (IsKeyDown(KEY_D)) {position.x += speed * dt * speedMultiplier; index = 3;}
    if (IsKeyDown(KEY_W)) {position.y -= speed * dt * speedMultiplier; index = 0;}
    if (IsKeyDown(KEY_S)) {position.y += speed * dt * speedMultiplier; index = 1;}

    if (IsKeyPressed(KEY_Q)) {hp.decrease(0.05f);}

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

void Player::applyEffect(const BuffEffect& effect) {
    buffSystem.addBuff(effect);
}

void Player::useSelectedItem() {
    ItemStack& stack = inventory.getSlot(inventory.selectedSlot);
    if (!stack.isEmpty()) {
        const Item& item = stack.getItem();
        item.onUse(*this);
        if (--stack.count == 0) {
            stack.id = ItemID::NONE;
        }
    }
}
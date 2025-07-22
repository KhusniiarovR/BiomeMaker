#include "Player.h"
#include "Constants/WorldConst.h"
#include "Constants/GraphicsConst.h"
#include "Constants/TilemapConst.h"

Player::Player(Vector2 initPos, const CollisionBase* collision, std::string fileName)
: Entity(initPos), 
collision(collision),
hp{{0.01f, 0.05f}, {0.15f, 0.05f}, RED, GRAY, "HEALTH", 10},
buffSystem(*this),
fileName(fileName + "/playerData"),
inventory(fileName + "/playerData"),
animation(entityTileSize, entityTileSize)
{
    hp.setProgress(1.0f);
    animation.addAnimation({"idle", 0, 4, 0.4f, true});
    animation.addAnimation({"walkDown", 4, 4, 0.1f, true});
}

void Player::update(float dt) {
    float moveSpeed = speed * dt * speedMultiplier;
    float dx = 0, dy = 0;
    bool moved = false;

    if (IsKeyDown(KEY_A)) { dx -= moveSpeed; animation.setFlip(true); animation.play("walkDown"); moved = true; } // left
    if (IsKeyDown(KEY_D)) { dx += moveSpeed; animation.setFlip(false); animation.play("walkDown"); moved = true; } // right
    if (IsKeyDown(KEY_W)) { dy -= moveSpeed; animation.play("walkDown"); moved = true; } // up
    if (IsKeyDown(KEY_S)) { dy += moveSpeed; animation.play("walkDown"); moved = true; } // down
    if (!moved) { animation.play("idle");}

    animation.update(dt);

    tryMove(dx, dy);

    if (IsKeyPressed(KEY_Q)) { hp.decrease(0.05f); }

    buffSystem.update(dt);
}

void Player::render(Renderer& renderer) const {
    animation.draw(renderer.getTexture("playerTilemap"), position);

    DrawCircleLinesV(position, handDistance * worldTileSize, YELLOW);
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

    if (!collision || !collision->checkCollision(newBox)) {
        position.x += dx;
        position.y += dy;
    } 
    else {
        newBox.x = oldBox.x + dx;
        newBox.y = oldBox.y;
        if (!collision || !collision->checkCollision(newBox)) {
            position.x += dx;
        }

        newBox.x = oldBox.x;
        newBox.y = oldBox.y + dy;
        if (!collision || !collision->checkCollision(newBox)) {
            position.y += dy;
        }
    }
}
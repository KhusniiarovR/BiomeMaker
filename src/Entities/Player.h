#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "Entity.h"
#include "Items/Inventory/Inventory.h"
#include "Core/Render/Renderer.h"
#include <string>
#include "Core/Render/Renderer.h"
#include "Utilities/Graphics/Bar.h"
#include "Items/Buffs/Buffs.h"
#include "Entities/BuffSystem.h"
#include "Constants/TilemapConst.h"

class Player : public Entity {
private:
    float speed;
    float speedMultiplier = 1.0f;
    Inventory inventory;
    ValueBar hp;
    int index = 1;
    ActiveBuffSystem buffSystem;
    std::function<bool(Rectangle)> collisionCallback;

public:
    Player(Vector2 init_pos);
    void update(float dt) override;
    void render(Renderer& renderer) const;

    Inventory& getInventory() { return inventory; }
    const Inventory& getInventory() const { return inventory; }
    [[nodiscard]] Vector2 getPosition() const;
    void giveItem(ItemID id, uint8_t count);

    float getSpeedMultiplier() const { return speedMultiplier; }
    void setSpeedMultiplier(float mult) { speedMultiplier = mult; }
    Rectangle getBoundingBox() const override;
    void setCollisionCallback(std::function<bool(Rectangle)> func) { collisionCallback = std::move(func); }
    
    void heal(float value);
    bool applyEffect(const BuffEffect& effect);
    void tryMove(float dx, float dy);
};

#endif //PLAYER_H

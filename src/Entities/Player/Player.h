#pragma once
#include "raylib.h"
#include "Entities/Entity.h"
#include "Core/Render/Renderer.h"
#include "Items/Inventory/Inventory.h"
#include "Utilities/Graphics/Bar.h"
#include "Entities/BuffSystem/Buffs.h"
#include "Entities/BuffSystem/BuffSystem.h"
#include "Constants/TilemapConst.h"
#include "Entities/Collisions/CollisionBase.h"
#include "Animations/Animation.h"
#include <string>

class Player : public Entity {
private:
    /* player characteristic */
    float speed = 80.0f, speedMultiplier = 1.0f; // speed
    int handDistance = 8; // items distance
    ValueBar hp; // health

    /* inventory */
    std::string fileName; // inventory file path
    Inventory inventory; // inventory
    
    /* others */
    BuffSystem buffSystem; // buffs
    const CollisionBase* collision; // collision
    Animation animation; // animation

public:
    Player(Vector2 initPos, const CollisionBase* collision, std::string fileName);
    void update(float dt) override;
    void render(Renderer& renderer) const;

    /* getters and setters */
    [[nodiscard]] Vector2 getPosition() const;
    float getSpeedMultiplier() const { return speedMultiplier; }
    void setSpeedMultiplier(float mult) { speedMultiplier = mult; }
    Inventory& getInventory() { return inventory; }
    const Inventory& getInventory() const { return inventory; }
    void giveItem(ItemID id, uint8_t count);
    Rectangle getBoundingBox() const override;
    [[nodiscard]] int getHandDistance() const { return handDistance; }
    const ValueBar& getHP() const { return hp; }
    
    /* effects */
    void heal(float value);
    bool applyEffect(const BuffEffect& effect);

private:    
    void tryMove(float dx, float dy);
};
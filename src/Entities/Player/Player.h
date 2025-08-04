#pragma once
#include "raylib.h"
#include "Entities/Entity.h"
#include "Core/Render/Renderer.h"
#include "Items/Inventory/Inventory.h"
#include "Utilities/Graphics/Bar.h"
#include "Entities/BuffSystem/Buffs.h"
#include "Entities/BuffSystem/BuffSystem.h"
#include "Constants/TilemapConst.h"
#include "Animations/Animation.h"
#include "Entities/Components/Attack/Knockback/Knockback.h"
#include <string>
#include "Entities/Collisions/Collision.h"

class Player : public Entity {
private:
    /* player characteristic */
    float speed = 80.0f, speedMultiplier = 1.0f; // speed
    int handDistance = 8; // items distance

    // health
    ValueBar hp; 
    float immuneFrames = 2.0f;
    Knockback knockback;

    /* inventory */
    std::string fileName; // inventory file path
    Inventory inventory; // inventory
    
    /* others */
    BuffSystem buffSystem; // buffs
    const Collision* collision = nullptr; // collision
    Animation animation; // animation

public:
    Player(Vector2 initPos, const Collision* collision, std::string fileName);
    void update(float dt) override;
    void render(Renderer& renderer) const;

    /* getters and setters */
    [[nodiscard]] Vector2 getPosition() const;                      // position
    float getSpeedMultiplier() const { return speedMultiplier; }    // speed
    void setSpeedMultiplier(float mult) { speedMultiplier = mult; } // speed
    Inventory& getInventory() { return inventory; }                 // inventory
    const Inventory& getInventory() const { return inventory; }     // inventory
    void giveItem(ItemID id, uint8_t count);                        // inventory item
    Rectangle getBoundingBox() const override;                      // hitbox
    [[nodiscard]] int getHandDistance() const { return handDistance; } // item use range
    const ValueBar& getHP() const { return hp; }                    // health
    bool isAlive() const { return hp.getProgress() > 0.0f; }        // health (alive)

    /* effects */
    void heal(float value);
    bool applyEffect(const BuffEffect& effect);
    
    /* others */
    void takeDamage(float amount, Vector2 source); // damage
    
private:    
    void tryMove(float dx, float dy);
};
#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "Entities/Entity.h"
#include "Items/Inventory/Inventory.h"
#include "Core/Render/Renderer.h"
#include <string>
#include "Core/Render/Renderer.h"
#include "Utilities/Graphics/Bar.h"
#include "Items/Buffs/Buffs.h"
#include "Entities/BuffSystem/BuffSystem.h"
#include "Constants/TilemapConst.h"
#include "Entities/Collisions/CollisionBase.h"
#include "Animations/Animation.h"

class Player : public Entity {
private:
    float speed = 80.0f;
    float speedMultiplier = 1.0f;
    int handDistance = 8;
    std::string fileName;
    Inventory inventory;
    ValueBar hp;
    ActiveBuffSystem buffSystem;
    const CollisionBase* collision;
    Animation animation;

public:
    Player(Vector2 initPos, const CollisionBase* collision, std::string fileName);
    void update(float dt) override;
    void render(Renderer& renderer) const;

    Inventory& getInventory() { return inventory; }
    const Inventory& getInventory() const { return inventory; }
    [[nodiscard]] Vector2 getPosition() const;
    void giveItem(ItemID id, uint8_t count);

    float getSpeedMultiplier() const { return speedMultiplier; }
    void setSpeedMultiplier(float mult) { speedMultiplier = mult; }
    Rectangle getBoundingBox() const override;
    [[nodiscard]] int getHandDistance() const { return handDistance; }
    const ValueBar& getHP() const { return hp; }
    
    void heal(float value);
    bool applyEffect(const BuffEffect& effect);
    void tryMove(float dx, float dy);
};

#endif //PLAYER_H

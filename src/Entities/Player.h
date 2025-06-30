#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "Entity.h"
#include "Items/Inventory/Inventory.h"
#include "Core/Render/Renderer.h"
#include <string>
#include "Core/Render/Renderer.h"
#include "Utilities/Graphics/Bar.h"

class Player : public Entity {
private:
    float speed;
    Inventory inventory;
    ValueBar hp;

public:
    Player(Vector2 init_pos);
    void update(float dt) override;
    void render(Renderer& renderer) const;

    int index = 0;

    [[nodiscard]] Vector2 getPosition() const;
    void giveItem(ItemID id, uint8_t count);
};

#endif //PLAYER_H

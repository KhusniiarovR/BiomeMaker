#include "Items/ItemBase/Item.h"
#include "Items/ItemBase/ItemUseContext.h"
#include "World/ChunkSystem/Object.h"
#include "World/World.h"
#include "raymath.h"
#include "Items/ItemBase/ObjectToItem.h"
#include "Utilities/Logger/Logger.h"

Item::Item(ItemID id, std::string name, std::string description)
    : id(id), name(std::move(name)), description(std::move(description)) {}

Item::~Item() = default;

Rectangle Item::getIconSourceRect() const {
    int index = static_cast<int>(id);
    int x = (index % ICONS_PER_ROW) * ICON_SIZE;
    int y = (index / ICONS_PER_ROW) * ICON_SIZE;
    return { (float)x, (float)y, (float)ICON_SIZE, (float)ICON_SIZE };
}

bool Item::onUse(const ItemUseContext& context) const {
    Vector2 playerPos = context.player.getPosition();
    Vector2 playerCenter = Vector2Add(playerPos, { entityTileSize / 2.0f, entityTileSize / 2.0f });
    Vector2 tilePos = { context.tileX * worldTileSize + worldTileSize / 2.0f,
                        context.tileY * worldTileSize + worldTileSize / 2.0f };
    float distSq = Vector2DistanceSqr(playerCenter, tilePos);
    float maxDist = context.player.getHandDistance() * worldTileSize;
    float maxDistSq = maxDist * maxDist;

    if (distSq > maxDistSq) return false;

    auto removed = context.world.removeObjectAt(context.tileX, context.tileY, this);
    if (removed) {
        std::vector<ItemID> drops = generateLootForObject(*removed);
        for (ItemID id : drops) {
            context.player.giveItem(id, 1);
        }
        return true;
    }

    return false;
}

bool Item::isPlaceable() const {
    return false;
}

bool Item::shouldConsumeOnUse() const {
    return true;
}

bool Item::canBreak(const ObjectProperties& objProp) const {
    return objProp.requiredTool == ToolType::NONE;
}

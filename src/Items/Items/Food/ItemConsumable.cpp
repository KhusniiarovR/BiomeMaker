#include "ItemConsumable.h"
#include "Items/ItemBase/ItemUseContext.h"
#include "Entities/Player.h"

ConsumableItem::ConsumableItem(BuffEffect eff)
    : effect(std::move(eff)) {}

bool ConsumableItem::onUse(const ItemUseContext& context) const {
    return context.player.applyEffect(effect);
}
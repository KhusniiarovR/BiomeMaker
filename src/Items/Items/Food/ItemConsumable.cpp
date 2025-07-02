#include "ItemConsumable.h"
#include "Entities/Player.h"

ConsumableItem::ConsumableItem(BuffEffect eff)
    : effect(std::move(eff))
{}

void ConsumableItem::onUse(Player& player) const {
    player.applyEffect(effect);
}
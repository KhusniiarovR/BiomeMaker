#pragma once
#include "Items/ItemBase/Item.h"
#include "Entities/BuffSystem/Buffs.h"
#include "Items/ItemBase/ItemUseContext.h"
#include "Entities/Player/Player.h"

struct ConsumableItem : public Item // base for food
{
    BuffEffect effect; // what effect food will give

    ConsumableItem(BuffEffect eff) : effect(std::move(eff)) {}
    bool onUse(const ItemUseContext& context) const { return context.player.applyEffect(effect); }
};
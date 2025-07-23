#ifndef CONSUMABLEITEM_H
#define CONSUMABLEITEM_H

#include "Items/ItemBase/Item.h"
#include "Entities/BuffSystem/Buffs.h"

struct ConsumableItem : public Item {
    BuffEffect effect;
    ConsumableItem(BuffEffect eff);
    bool onUse(const ItemUseContext& context) const override;
};

#endif // CONSUMABLEITEM_H

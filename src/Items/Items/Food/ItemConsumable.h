#ifndef CONSUMABLEITEM_H
#define CONSUMABLEITEM_H

#include "Items/ItemBase/Item.h"
#include "Items/Buffs/Buffs.h"

struct ConsumableItem : public Item {
    BuffEffect effect;
    ConsumableItem(BuffEffect eff);
    void onUse(Player& player) const override;
};

#endif // CONSUMABLEITEM_H

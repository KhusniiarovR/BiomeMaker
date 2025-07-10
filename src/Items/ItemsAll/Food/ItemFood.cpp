#include "Items/ItemBase/Item.h"
#include "ItemFood.h"

ItemBerry::ItemBerry() : ConsumableItem(BuffEffect(BuffType::Heal, 0.1f)) {
    id = ItemID::BERRY;
    name = "Berry";
    description = "Looks tasty";
}
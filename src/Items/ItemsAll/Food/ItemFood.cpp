#include "Items/ItemBase/Item.h"
#include "ItemFood.h"

ItemBerry::ItemBerry() : ConsumableItem({BuffType::Heal, 0.1f, 0.0f}) // type of buff + how strong + duration 
{
    id = ItemID::BERRY;
    name = "Berry";
    description = "Looks tasty";
}
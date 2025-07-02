#include "Items/ItemBase/Item.h"
#include "ItemFood.h"
#include "Items/ItemRegister/ItemRegister.h"
#include "Entities/Player.h"

void registerAllFood() {
    ItemRegister::get().registerItem(std::make_unique<ItemBerry>());
}


ItemBerry::ItemBerry() : ConsumableItem(BuffEffect(BuffType::Heal, 0.1f)) {
    id = ItemID::BERRY;
    name = "Berry";
    description = "Looks tasty";
}
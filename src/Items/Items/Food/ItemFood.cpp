#include "Items/ItemRegister.h"
#include "ItemFood.h"
#include "Entities/Player.h"

void registerAllFood() {
    ItemRegister::get().registerItem(std::make_unique<ItemBerry>());
}

ItemBerry::ItemBerry() {
    id = ItemID::BERRY;
    name = "Berry";
    description = "looks tasty";
}

void ItemBerry::onUse(Player& player) const {
    player.heal(0.1f);
}
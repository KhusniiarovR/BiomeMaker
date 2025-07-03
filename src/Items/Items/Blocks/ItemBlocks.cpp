#include "Items/ItemRegister/ItemRegister.h"
#include "ItemBlocks.h"

ItemWood::ItemWood() : ItemPlaceable(ObjectType::WoodenWall) {
    id = ItemID::WOOD;
    name = "Wood";
    description = "dropped from tree";
}

ItemStone::ItemStone() : ItemPlaceable(ObjectType::Rock) {
    id = ItemID::STONE;
    name = "Stone";
    description = "dropped from big stone";
}
#include "Items/ItemRegister/ItemRegister.h"
#include "ItemBlocks.h"

ItemWood::ItemWood() : ItemPlaceable(ObjectType::OBJECT_WOODEN_WALL) {
    id = ItemID::WOOD;
    name = "Wood";
    description = "dropped from tree";
}

ItemStone::ItemStone() : ItemPlaceable(ObjectType::OBJECT_ROCK) {
    id = ItemID::STONE;
    name = "Stone";
    description = "dropped from big stone";
}
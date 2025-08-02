#include "ItemBlocks.h"

ItemWood::ItemWood() : ItemPlaceable(ObjectType::WOODEN_WALL) 
{
    id = ItemID::WOOD;
    name = "Wood";
    description = "dropped from tree";
}

ItemStone::ItemStone() : ItemPlaceable(ObjectType::ROCK) 
{
    id = ItemID::STONE;
    name = "Stone";
    description = "dropped from big stone";
} 
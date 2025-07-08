#include "ItemTools.h"


ItemAxe::ItemAxe() : ItemToolBase(ItemID::AXE, "Axe", "Used to chop trees", 5) {}
bool ItemAxe::canBreak(ObjectType type) const {
    return type == ObjectType::OBJECT_TREE || type == ObjectType::OBJECT_WOODEN_WALL;
}

ItemPickaxe::ItemPickaxe() : ItemToolBase(ItemID::PICKAXE, "Pickaxe", "Used to break stones", 50) {}
bool ItemPickaxe::canBreak(ObjectType type) const {
    return type == ObjectType::OBJECT_ROCK || type == ObjectType::OBJECT_BUSH;
}
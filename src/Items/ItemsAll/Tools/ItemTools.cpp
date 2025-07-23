#include "ItemTools.h"

ItemAxe::ItemAxe() : ItemToolBase(ItemID::AXE, "Axe", "Used to chop trees", ToolType::AXE, 1, 5) {}
ItemPickaxe::ItemPickaxe() : ItemToolBase(ItemID::PICKAXE, "Pickaxe", "Used to break stones", ToolType::PICKAXE, 1, 15) {}

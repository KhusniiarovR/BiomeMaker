#ifndef OBJECT_TO_ITEM_H
#define OBJECT_TO_ITEM_H

#include "Item.h"
#include "World/ChunkSystem/Object.h"
#include <random>

struct LootDrop {
    ItemID id;
    float probability;
    int minAmount = 1;
    int maxAmount = 1;
};

const std::unordered_map<ObjectType, std::vector<LootDrop>> objectLootTable = {
    { ObjectType::OBJECT_TREE, { { ItemID::WOOD, 1.0f, 1, 3 } } },
    { ObjectType::OBJECT_BUSH, { { ItemID::BERRY, 0.5f, 1, 2 } } },
    { ObjectType::OBJECT_ROCK, { { ItemID::STONE, 1.0f, 1, 2 } } }
};

inline std::vector<ItemID> generateLootForObject(ObjectType type) {
    std::vector<ItemID> result;
    auto it = objectLootTable.find(type);
    if (it == objectLootTable.end()) return result;

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> chance(0.0f, 1.0f);

    for (const LootDrop& drop : it->second) {
        if (chance(rng) < drop.probability) {
            int amount = drop.minAmount;
            if (drop.maxAmount > drop.minAmount) {
                std::uniform_int_distribution<int> amt(drop.minAmount, drop.maxAmount);
                amount = amt(rng);
            }

            for (int i = 0; i < amount; ++i)
                result.push_back(drop.id);
        }
    }

    return result;
}

#endif // OBJECT_TO_ITEM_H
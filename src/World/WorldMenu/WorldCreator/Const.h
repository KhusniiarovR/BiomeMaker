#pragma once
#include "World/ChunkSystem/Biome.h"

constexpr int extraCentersInner = 4;
constexpr int extraCentersOuter = 10;
constexpr int innerRadius = 200;
constexpr int ringWidth = 30;
constexpr int outerRadius = innerRadius + ringWidth;

const uint8_t errorBiome = BIOME_UNKNOWN;
const uint8_t centerBiome = BIOME_FOREST;
const std::vector<uint8_t> innerBiomes = {BIOME_WINTER, BIOME_SAND, BIOME_SWAMP, BIOME_VOLCANO};
const std::vector<uint8_t> outerBiomes = {BIOME_GLASS, BIOME_BEACH, BIOME_CRYSTAL, BIOME_MOUNTAIN};
const uint8_t ringBiome = BIOME_EXIT;

struct BiomeCenter {
    int x, y;
    uint8_t type;
};
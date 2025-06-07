#ifndef BIOME_H
#define BIOME_H

#include "raylib.h"
#include <vector>

struct Biome {
    Color color;
};

const std::vector<Biome> BIOMES = {
    { Color{ 255, 0, 255, 255 } },      // unknown
    { Color{ 34, 170, 34, 255 } },      // Forest
    { Color{ 50, 50, 175, 255 } },      // Ocean
    { Color{ 210, 210, 210, 255 } },    // Winter
    { Color{ 220, 220, 120, 255 } },    // Sand
    { Color{ 110, 20, 20, 255 } }       // Volcano
};

#endif //BIOME_H

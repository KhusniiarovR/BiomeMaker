#ifndef BIOME_H
#define BIOME_H

#include "raylib.h"
#include <vector>

struct Biome {
    Color color;
};

const std::vector<Biome> BIOMES = {
    { Color{ 34, 139, 34, 255 } },      // Forest
    { Color{ 194, 178, 128, 255 } },    // Desert
    { Color{ 100, 100, 100, 255 } }     // Mountain
};

#endif //BIOME_H

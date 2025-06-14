#ifndef BIOME_H
#define BIOME_H

#include "raylib.h"
#include <vector>

struct Biome {
    Color color;
};

// todo merge biome color and symbol

const char errorBiome = 'U';
const char centerBiome = 'F';
const std::vector<char> innerBiomes = { 'O', 'W', 'S', 'V' };
const std::vector<char> outerBiomes = { 'G', 'B', 'C', 'M' };
const char ringBiome = 'E';

const char BIOME_SYMBOLS[] = { 'F', 'O', 'W', 'S', 'V', 'G', 'B', 'C', 'M', 'E' };

const std::vector<Biome> BIOMES = {
    { Color{ 255, 0, 255, 255 } },      // Unknown
    { Color{ 34, 170, 34, 255 } },      // Forest
    { Color{ 50, 50, 175, 255 } },      // Ocean
    { Color{ 210, 210, 210, 255 } },    // Winter
    { Color{ 220, 220, 120, 255 } },    // Sand
    { Color{ 110, 20, 20, 255 } },      // Volcano
    { Color{RED}},                      // Glass
    { Color{SKYBLUE}},                  // Beach
    { Color{PURPLE}},                   // Crystal
    { Color{GRAY}},                     // Mountain
    { Color{BLACK}}                     // Exit Block
};



#endif //BIOME_H

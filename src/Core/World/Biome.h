#ifndef BIOME_H
#define BIOME_H

#include "raylib.h"
#include <vector>

struct Biome {
    uint8_t id;
    char symbol;
    std::vector<std::pair<uint8_t, float>> tileVariants;
};



const char errorBiome = 'U';
const char centerBiome = 'F';
const std::vector<char> innerBiomes = { 'O', 'W', 'S', 'V' };
const std::vector<char> outerBiomes = { 'G', 'B', 'C', 'M' };
const char ringBiome = 'E';

const char BIOME_SYMBOLS[] = { 'F', 'W', 'S', 'O', 'V', 'E', 'G', 'B', 'C', 'M'};

const std::vector<Biome> BIOMES = {
    // Unknown
    {
        0, 'U',
        {
            { 0, 1.0f }
        }
    },
    // Forest
    {
        1, 'F',
        {
            { 1, 0.970f },
            { 2, 0.015f },
            { 3, 0.010f },
            { 4, 0.005f }
            
        }
    },
    // Winter
    {
        2, 'W',
        {
            { 5, 0.970f },
            { 6, 0.015f },
            { 7, 0.010f },
            { 8, 0.005f }
        }
    },
    // Sand
    {
        3, 'S',
        {
            {  9, 0.970f },
            { 10, 0.015f },
            { 11, 0.010f },
            { 12, 0.005f }
        }
    },
    // Swamp
    {
        4, 'O',
        {
            { 13, 0.970f },
            { 14, 0.015f },
            { 15, 0.010f },
            { 16, 0.005f }
        }
    },
    // Volcano
    {
        5, 'V',
        {
            { 17, 0.995f},
            { 18, 0.005f}
        }
    },
    // Exit
    {
        6, 'E',
        {
            { 19, 1.0f }
        }
    },
    // Glass
    {
        7, 'G',
        {
            { 20, 1.0f }
        }
    },
    // Beach
    {
        8, 'B',
        {
            { 21, 1.0f }
        }
    },
    // Crystal
    {
        9, 'C',
        {
            { 22, 1.0f }
        }
    },
    // Mountain
    {
        10, 'M',
        {
            { 23, 1.0f }
        }
    },
};



#endif //BIOME_H

#ifndef BIOME_H
#define BIOME_H

#include <vector>

struct Biome {
    uint8_t id;
    std::vector<std::pair<uint8_t, float>> tileVariants;
};

constexpr uint8_t BIOME_UNKNOWN = 0;
constexpr uint8_t BIOME_FOREST = 1;
constexpr uint8_t BIOME_WINTER = 2;
constexpr uint8_t BIOME_SAND = 3;
constexpr uint8_t BIOME_SWAMP = 4;
constexpr uint8_t BIOME_VOLCANO = 5;
constexpr uint8_t BIOME_EXIT = 6;
constexpr uint8_t BIOME_GLASS = 7;
constexpr uint8_t BIOME_BEACH = 8;
constexpr uint8_t BIOME_CRYSTAL = 9;
constexpr uint8_t BIOME_MOUNTAIN = 10;

const std::vector<Biome> BIOMES = {
    {
        BIOME_UNKNOWN,
        {
            { 0, 1.0f }
        }
    },
    {
        BIOME_FOREST,
        {
            { 1, 0.970f },
            { 2, 0.015f },
            { 3, 0.010f },
            { 4, 0.005f }
            
        }
    },
    {
        BIOME_WINTER,
        {
            { 5, 0.970f },
            { 6, 0.015f },
            { 7, 0.010f },
            { 8, 0.005f }
        }
    },
    {
        BIOME_SAND,
        {
            {  9, 0.970f },
            { 10, 0.015f },
            { 11, 0.010f },
            { 12, 0.005f }
        }
    },
    {
        BIOME_SWAMP,
        {
            { 13, 0.970f },
            { 14, 0.015f },
            { 15, 0.010f },
            { 16, 0.005f }
        }
    },
    {
        BIOME_VOLCANO,
        {
            { 17, 0.995f},
            { 18, 0.005f}
        }
    },
    {
        BIOME_EXIT,
        {
            { 19, 1.0f }
        }
    },
    {
        BIOME_GLASS,
        {
            { 20, 1.0f }
        }
    },
    {
        BIOME_BEACH,
        {
            { 21, 1.0f }
        }
    },
    {
        BIOME_CRYSTAL,
        {
            { 22, 1.0f }
        }
    },
    {
        BIOME_MOUNTAIN,
        {
            { 23, 1.0f }
        }
    },
};



#endif //BIOME_H

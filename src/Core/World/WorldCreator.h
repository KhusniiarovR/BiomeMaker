#ifndef WORLDCREATOR_H
#define WORLDCREATOR_H

#include <vector>
#include <cstdlib>
#include <string>
#include <filesystem>

class WorldCreator {
private:
    static constexpr int extraCentersInner = 4;
    static constexpr int extraCentersOuter = 10;
    static constexpr int innerRadius = 200;
    static constexpr int ringWidth = 30;
    static constexpr int outerRadius = innerRadius + ringWidth;

// todo configure world size
// merge biomes here and biome.h

    std::string worldName;

    const char errorBiome = 'U';
    const char centerBiome = 'F';
    const std::vector<char> innerBiomes = { 'O', 'W', 'S', 'V' };
    const std::vector<char> outerBiomes = { 'G', 'B', 'C', 'M' };
    const char ringBiome = 'E';

    struct BiomeCenter {
        int x, y;
        char type;
    };

public:
    WorldCreator();
    ~WorldCreator();

    void generate(std::string worldName); // random seed
    void generate(int seed, std::string worldName);

private:
    static int dist2(int x1, int y1, int x2, int y2);

    void save_world_rle(const std::vector<std::vector<char>>& world);
};


#endif //WORLDCREATOR_H

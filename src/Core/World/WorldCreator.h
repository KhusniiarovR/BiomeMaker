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

    struct BiomeCenter {
        int x, y;
        char type;
    };

public:
    WorldCreator();
    ~WorldCreator();

    void generate(); // random name and seed
    void generate(std::string worldName); // random seed
    void generate(int seed, std::string worldName);

private:
    static int dist2(int x1, int y1, int x2, int y2);

    std::string getName();

    void write_rle_chunk(std::ofstream& out,const std::vector<std::vector<char>>& data,int startX, int startY);
    void save_world_rle(const std::vector<std::vector<char>>& world, const std::vector<std::vector<char>>& objects);
};


#endif //WORLDCREATOR_H

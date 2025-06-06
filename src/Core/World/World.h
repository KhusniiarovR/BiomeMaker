#ifndef WORLD_H
#define WORLD_H

#include "Constants/WorldConst.h"
#include "Chunk.h"
#include "Utilities/World/PairHash.h"
#include <unordered_map>

class World {
    std::unordered_map<std::pair<int, int>, Chunk, PairHash> chunks;

public:
    void update(Vector2 playerPos);
    void render() const;
    [[nodiscard]] int getChunkCount() const;
};

#endif //WORLD_H

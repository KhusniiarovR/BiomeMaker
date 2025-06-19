#ifndef WORLD_H
#define WORLD_H

#include "Chunk.h"
#include "Utilities/World/PairHash.h"
#include <unordered_map>
#include "Core/Render/Renderer.h"

class World {
    std::unordered_map<std::pair<int, int>, Chunk, PairHash> chunks;
    std::vector<ChunkHeader> headers;
    std::ifstream worldFile;
    std::string filename;

public:
    World(const std::string &filename);

    void LoadHeaders();

    void update(Vector2 playerPos);
    void render(Renderer& renderer) const;
    [[nodiscard]] int getChunkCount() const;
};

#endif //WORLD_H

#ifndef WORLD_H
#define WORLD_H

#include "Chunk.h"
#include "Utilities/World/PairHash.h"
#include <unordered_map>
#include "Core/Render/Renderer.h"
#include "WorldChanger.h"

class World {
private:
    std::unordered_map<std::pair<int, int>, Chunk, PairHash> chunks;
    std::vector<ChunkHeader> headers;
    std::ifstream worldFile;
    std::string filename;
    
    WorldChanger worldChanger;

public:
    World(const std::string &filename);
    ~World();

    void LoadHeaders();

    void update(Vector2 playerPos, Camera2D& camera);
    void render(Renderer& renderer) const;
    [[nodiscard]] int getChunkCount() const;

private:
    void updateChunks(Vector2 playerPos);
    bool removeObjectAt(int worldX, int worldY);
};

#endif //WORLD_H

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
    ~World();

    void LoadHeaders();

    void update(Vector2 playerPos, Camera2D& camera);
    void render(Renderer& renderer) const;
    [[nodiscard]] int getChunkCount() const;

private:
    void updateChunks(Vector2 playerPos);
    bool removeObjectAt(int worldX, int worldY);
    void overwrite_chunk_rle(int cx, int cy, const Chunk& chunk);
    void write_rle_data(std::ostream& out, const std::vector<std::vector<char>>& data);
    char objectToSymbol(const Object& obj);
    char biomeToSymbolFromTileIndex(uint8_t tileIndex);
    void regenerate_and_save_full_world();
};

#endif //WORLD_H

#ifndef WORLDCHANGER_H
#define WORLDCHANGER_H

#include <unordered_map>
#include "Chunk.h"
#include "Utilities/World/PairHash.h"
#include "Core/Render/Renderer.h"

class ChunkSystem {
private:    
    std::unordered_map<std::pair<int, int>, Chunk, PairHash>& chunks;
    std::vector<ChunkHeader> headers;
    std::ifstream worldFile;
    std::string filename;

public:
    ChunkSystem(std::unordered_map<std::pair<int, int>, Chunk, PairHash>& chunks,
                 const std::string& filename);
    ~ChunkSystem();             

    void update(Vector2& playerPos);
    void render(Renderer& renderer) const;
    
    private:
    void LoadHeaders();
    
    void updateChunks(Vector2& playerPos);

    void overwriteChunk(int cx, int cy, const Chunk& chunk);
    void writeData(std::ostream& out, const std::vector<std::vector<uint8_t>>& data);
    void saveFullWorld();
};







#endif // WORLDCHANGER_H
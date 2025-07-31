#pragma once
#include <unordered_map>
#include "Chunk.h"
#include "Utilities/World/PairHash.h"
#include "Core/Render/Renderer.h"

class ChunkSystem { // chunk manager
private:    
    std::unordered_map<std::pair<int, int>, Chunk, PairHash>& chunks;
    std::vector<ChunkHeader> headers;
    std::ifstream worldFile;
    std::string filename;

public:
    ChunkSystem(std::unordered_map<std::pair<int, int>, Chunk, PairHash>& chunks, const std::string& filename);
    ~ChunkSystem();             
    void update(Vector2& playerPos);
    void render(Renderer& renderer) const;
    
private:
    void LoadHeaders();

    void updateChunks(Vector2& playerPos);
    
    // saving
    void overwriteChunk(int cx, int cy, const Chunk& chunk);
    void writeData(std::ostream& out, const std::vector<std::vector<uint8_t>>& data);
    void saveFullWorld();
    void writeObjectsChunk(std::ostream& out, const std::vector<Object>& objects, int startTileX, int startTileY, int tileSize);  
    
    uint8_t resolveBiomeId(uint8_t tileIndex) const;
};
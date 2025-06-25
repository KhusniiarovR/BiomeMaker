#ifndef WORLDCHANGER_H
#define WORLDCHANGER_H

#include <unordered_map>
#include "Chunk.h"
#include "Utilities/World/PairHash.h"


class WorldChanger {
private:    
    std::unordered_map<std::pair<int, int>, Chunk, PairHash>& chunks;
    std::vector<ChunkHeader>& headers;
    std::ifstream& worldFile;
    std::string filename;
public:
    WorldChanger(std::unordered_map<std::pair<int, int>, Chunk, PairHash>& chunks,
                 std::vector<ChunkHeader>& headers, 
                 std::ifstream& worldFile,
                 const std::string& filename);
    void overwriteChunk(int cx, int cy, const Chunk& chunk);
    
private:
    void writeData(std::ostream& out, const std::vector<std::vector<char>>& data);
    char objectToSymbol(const Object& obj);
    char biomeToSymbolFromTileIndex(uint8_t tileIndex);
    void saveFullWorld();
};







#endif // WORLDCHANGER_H
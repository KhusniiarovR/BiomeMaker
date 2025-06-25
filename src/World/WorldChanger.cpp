#include "WorldChanger.h"
#include <sstream>

WorldChanger::WorldChanger(
    std::unordered_map<std::pair<int, int>, Chunk, PairHash>& chunks,
    std::vector<ChunkHeader>& headers, 
    std::ifstream& worldFile,
    const std::string& filename) : chunks(chunks), headers(headers), worldFile(worldFile), filename(filename) 
{

}

void WorldChanger::overwriteChunk(int cx, int cy, const Chunk& chunk) {
    std::fstream file(filename + "/world.dat", std::ios::binary | std::ios::in | std::ios::out);
    if (!file.is_open()) {
        std::cerr << "Failed to open world.dat for overwriting chunk\n";
        return;
    }

    int index = cy * numberOfChunks + cx;
    file.seekg(index * sizeof(ChunkHeader), std::ios::beg);

    ChunkHeader header{};
    file.read(reinterpret_cast<char*>(&header), sizeof(ChunkHeader));

    if (file.fail()) {
        std::cerr << "Failed to read ChunkHeader at index " << index << "\n";
        file.close();
        return;
    }

    std::vector<std::vector<char>> biomeAsChar(chunkSize, std::vector<char>(chunkSize));
    std::vector<std::vector<char>> objectsAsChar(chunkSize, std::vector<char>(chunkSize));

    for (int y = 0; y < chunkSize; ++y) {
        for (int x = 0; x < chunkSize; ++x) {
            biomeAsChar[y][x] = biomeToSymbolFromTileIndex(chunk.tiles[y][x]);
            objectsAsChar[y][x] = objectToSymbol(chunk.objectTiles[y][x]);
        }
    }

    std::stringstream biomeBuffer;
    writeData(biomeBuffer, biomeAsChar);
    std::string biomeData = biomeBuffer.str();

    std::stringstream objectBuffer;
    writeData(objectBuffer, objectsAsChar);
    std::string objectData = objectBuffer.str();

    if (biomeData.size() > header.reservedSizeBiome) {
        std::cerr << "Biome RLE data exceeds reserved space at chunk (" << cx << "," << cy << ")\n";
        saveFullWorld();
        file.close();
        return;
    }

    if (objectData.size() > header.reservedSizeObject) {
        std::cerr << "Object RLE data exceeds reserved space at chunk (" << cx << "," << cy << ")\n";
        saveFullWorld();
        file.close();
        return;
    }

    file.seekp(header.offsetBiome, std::ios::beg);
    file.write(biomeData.data(), biomeData.size());
    file.write(std::string(header.reservedSizeBiome - biomeData.size(), '~').c_str(),
               header.reservedSizeBiome - biomeData.size());

    file.seekp(header.offsetObject, std::ios::beg);
    file.write(objectData.data(), objectData.size());
    file.write(std::string(header.reservedSizeObject - objectData.size(), '~').c_str(),
               header.reservedSizeObject - objectData.size());

    ChunkHeader newHeader = {
        header.offsetBiome,
        static_cast<uint32_t>(biomeData.size()),
        header.reservedSizeBiome,
        header.offsetObject,
        static_cast<uint32_t>(objectData.size()),
        header.reservedSizeObject
    };

    file.seekp(index * sizeof(ChunkHeader), std::ios::beg);
    file.write(reinterpret_cast<const char*>(&newHeader), sizeof(ChunkHeader));
    file.close();
}

void WorldChanger::writeData(std::ostream& out, const std::vector<std::vector<char>>& data) {
    char current = data[0][0];
    uint8_t count = 1;

    for (int y = 0; y < chunkSize; ++y) {
        for (int x = 0; x < chunkSize; ++x) {
            if (x == 0 && y == 0) continue;
            char next = data[y][x];
            if (next == current && count < 255) {
                count++;
            } else {
                out.write(reinterpret_cast<const char*>(&count), 1);
                out.write(&current, 1);
                current = next;
                count = 1;
            }
        }
    }

    out.write(reinterpret_cast<const char*>(&count), 1);
    out.write(&current, 1);
}

char WorldChanger::objectToSymbol(const Object& obj) {
    switch (obj.type) {
        case ObjectType::Tree: return 'T';
        case ObjectType::Rock: return 'R';
        case ObjectType::Bush: return 'B';
        case ObjectType::None: return ' ';
        default: {
            std::cerr << "objectToSymbol: unknown ObjectType\n";
            return '?';
        }
    }
}

char WorldChanger::biomeToSymbolFromTileIndex(uint8_t tileIndex) {
    for (const auto& biome : BIOMES) {
        for (const auto& [tile, _] : biome.tileVariants) {
            if (tile == tileIndex) {
                return biome.symbol;
            }
        }
    }
    std::cerr << "biomeToSymbolFromTileIndex: unknown tile index " << (int)tileIndex << "\n";
    return BIOMES[0].symbol;
}

void WorldChanger::saveFullWorld() {
    std::string oldFile = filename + "/world.dat";
    std::string newFile = filename + "/world_temp.dat";

    std::ifstream in(oldFile, std::ios::binary);
    std::ofstream out(newFile, std::ios::binary | std::ios::trunc);

    if (!in.is_open() || !out.is_open()) {
        std::cerr << "Failed to open world file(s)\n";
        return;
    }

    int total_chunks = numberOfChunks * numberOfChunks;
    std::vector<ChunkHeader> oldHeaders(total_chunks);
    std::vector<ChunkHeader> newHeaders(total_chunks);

    in.read(reinterpret_cast<char*>(oldHeaders.data()), total_chunks * sizeof(ChunkHeader));
    if (in.gcount() != total_chunks * sizeof(ChunkHeader)) {
        std::cerr << "Corrupted or incomplete header section\n";
        return;
    }

    out.seekp(total_chunks * sizeof(ChunkHeader), std::ios::beg);

    for (int cy = 0; cy < numberOfChunks; ++cy) {
        for (int cx = 0; cx < numberOfChunks; ++cx) {
            int index = cy * numberOfChunks + cx;
            if (index < 0 || index >= total_chunks) continue;

            auto chunkIt = chunks.find({cx, cy});
            std::streampos biomeOffset = out.tellp();

            if (chunkIt != chunks.end() && chunkIt->second.isModified) {
                const Chunk& chunk = chunkIt->second;
                std::vector<std::vector<char>> biomeAsChar(chunkSize, std::vector<char>(chunkSize));
                for (int y = 0; y < chunkSize; ++y)
                    for (int x = 0; x < chunkSize; ++x)
                        biomeAsChar[y][x] = biomeToSymbolFromTileIndex(chunk.tiles[y][x]);

                std::stringstream biomeBuffer;
                writeData(biomeBuffer, biomeAsChar);
                std::string biomeData = biomeBuffer.str();

                uint32_t biomeReserve = biomeData.size() + reserveSizeBiome;
                out.write(biomeData.data(), biomeData.size());
                out.write(std::string(biomeReserve - biomeData.size(), '~').c_str(), biomeReserve - biomeData.size());

                std::vector<std::vector<char>> objectAsChar(chunkSize, std::vector<char>(chunkSize));
                for (int y = 0; y < chunkSize; ++y)
                    for (int x = 0; x < chunkSize; ++x)
                        objectAsChar[y][x] = objectToSymbol(chunk.objectTiles[y][x]);

                std::stringstream objectBuffer;
                writeData(objectBuffer, objectAsChar);
                std::string objectData = objectBuffer.str();

                std::streampos objectOffset = out.tellp();
                uint32_t objectReserve = objectData.size() + reserveSizeObject;
                out.write(objectData.data(), objectData.size());
                out.write(std::string(objectReserve - objectData.size(), '~').c_str(), objectReserve - objectData.size());

                newHeaders[index] = {
                    static_cast<uint32_t>(biomeOffset),
                    static_cast<uint32_t>(biomeData.size()),
                    biomeReserve,
                    static_cast<uint32_t>(objectOffset),
                    static_cast<uint32_t>(objectData.size()),
                    objectReserve
                };
            } else {
                const ChunkHeader& old = oldHeaders[index];
                if (old.reservedSizeBiome == 0 || old.reservedSizeObject == 0) continue;

                std::vector<char> biomeData(old.reservedSizeBiome);
                in.seekg(old.offsetBiome, std::ios::beg);
                if (!in.read(biomeData.data(), old.reservedSizeBiome)) continue;
                out.write(biomeData.data(), old.reservedSizeBiome);

                std::vector<char> objectData(old.reservedSizeObject);
                in.seekg(old.offsetObject, std::ios::beg);
                if (!in.read(objectData.data(), old.reservedSizeObject)) continue;
                std::streampos objectOffset = out.tellp();
                out.write(objectData.data(), old.reservedSizeObject);

                newHeaders[index] = {
                    static_cast<uint32_t>(biomeOffset),
                    old.dataSizeBiome,
                    old.reservedSizeBiome,
                    static_cast<uint32_t>(objectOffset),
                    old.dataSizeObject,
                    old.reservedSizeObject
                };
            }
        }
    }

    out.seekp(0, std::ios::beg);
    out.write(reinterpret_cast<const char*>(newHeaders.data()), total_chunks * sizeof(ChunkHeader));

    in = std::ifstream();
    out = std::ofstream();

    try {
        std::filesystem::copy_file(newFile, oldFile, std::filesystem::copy_options::overwrite_existing);
        std::filesystem::remove(newFile);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem error during file copy+replace: " << e.what() << "\n";
        return;
    }
}

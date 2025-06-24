#include "World.h"
#include "Constants/GraphicsConst.h"
#include <ranges>
#include "Constants/WorldConst.h"
#include <sstream>

World::World(const std::string &filename) : filename(filename) {
    LoadHeaders();
}

World::~World() {
    for (const auto& [coord, chunk] : chunks) {
        if (chunk.isModified) {
            int cx = coord.first;
            int cy = coord.second;
            overwrite_chunk_rle(cx, cy, chunk);
        }
    }
}

void World::LoadHeaders() {
    worldFile.open(filename + "/world.dat", std::ios::binary);
    if (!worldFile.is_open()) {
        std::cerr << "Error: can't open world.dat\n";
        return;
    }

    int totalChunks = worldSize;
    headers.resize(totalChunks);
    worldFile.read(reinterpret_cast<char*>(headers.data()), totalChunks * sizeof(ChunkHeader));
}

void World::update(Vector2 playerPos, Camera2D& camera) {
    updateChunks(playerPos);

    if (IsKeyPressed(KEY_E)) {
    Vector2 mouseScreen = GetMousePosition();
    Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);

    int tileX = static_cast<int>(mouseWorld.x / tileSize);
    int tileY = static_cast<int>(mouseWorld.y / tileSize);

    int playerTileX = static_cast<int>(playerPos.x / tileSize);
    int playerTileY = static_cast<int>(playerPos.y / tileSize);

    int dx = tileX - playerTileX;
    int dy = tileY - playerTileY;

    if (dx * dx + dy * dy <= handDistance*handDistance) {
        removeObjectAt(tileX, tileY);
    }
}
}

void World::render(Renderer& renderer) const {
    Texture2D& tilemap = renderer.getTexture("tilemap");
    for (const auto& chunk : chunks | std::views::values) {
        chunk.Draw(tilemap);
    }
}

void World::updateChunks(Vector2 playerPos) {
    int playerChunkX = static_cast<int>(playerPos.x / chunkPixelSize);
    int playerChunkY = static_cast<int>(playerPos.y / chunkPixelSize);

    const int preloadAhead = renderDistance / 2;
    const int preloadBehind = renderDistance - preloadAhead;

    int loadStartX = playerChunkX - preloadBehind;
    int loadEndX   = playerChunkX + preloadAhead + 1;

    int loadStartY = playerChunkY - preloadBehind;
    int loadEndY   = playerChunkY + preloadAhead + 1;

    for (int y = loadStartY; y < loadEndY; ++y) {
        for (int x = loadStartX; x < loadEndX; ++x) {
            auto key = std::make_pair(x, y);
            if (chunks.find(key) == chunks.end()) {
                chunks.emplace(key, Chunk(x, y, headers, worldFile));
            }
        }
    }

    int unloadStartX = loadStartX - unloadMargin;
    int unloadEndX   = loadEndX + unloadMargin;
    int unloadStartY = loadStartY - unloadMargin;
    int unloadEndY   = loadEndY + unloadMargin;

    auto it = chunks.begin();
    while (it != chunks.end()) {
        auto [chunkX, chunkY] = it->first;
        if (chunkX < unloadStartX || chunkX >= unloadEndX ||
            chunkY < unloadStartY || chunkY >= unloadEndY) {
            if (it->second.isModified) {
                overwrite_chunk_rle(chunkX, chunkY, it->second);
                // it->second.isModified = false; anyway delete
            }
            it = chunks.erase(it);
        } 
        else {
            ++it;
        }
    }
}

bool World::removeObjectAt(int worldX, int worldY) {
    int chunkX = worldX / chunkSize;
    int chunkY = worldY / chunkSize;

    int localX = worldX % chunkSize;
    int localY = worldY % chunkSize;

    auto it = chunks.find({chunkX, chunkY});
    if (it == chunks.end()) return false;

    Chunk& chunk = it->second;
    Object& obj = chunk.objectTiles[localY][localX];

    if (obj.type != ObjectType::None) {
        obj.type = ObjectType::None;
        chunks.at({chunkX, chunkY}).isModified = true; 
        return true;
    }
    return false;
}

int World::getChunkCount() const {
    return chunks.size();
}

void World::overwrite_chunk_rle(int cx, int cy, const Chunk& chunk) {
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
    write_rle_data(biomeBuffer, biomeAsChar);
    std::string biomeData = biomeBuffer.str();

    std::stringstream objectBuffer;
    write_rle_data(objectBuffer, objectsAsChar);
    std::string objectData = objectBuffer.str();

    if (biomeData.size() > header.reservedSizeBiome) {
        std::cerr << "Biome RLE data exceeds reserved space at chunk (" << cx << "," << cy << ")\n";
        regenerate_and_save_full_world();
        file.close();
        return;
    }

    if (objectData.size() > header.reservedSizeObject) {
        std::cerr << "Object RLE data exceeds reserved space at chunk (" << cx << "," << cy << ")\n";
        regenerate_and_save_full_world();
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

void World::write_rle_data(std::ostream& out, const std::vector<std::vector<char>>& data) {
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

char World::objectToSymbol(const Object& obj) {
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

char World::biomeToSymbolFromTileIndex(uint8_t tileIndex) {
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

void World::regenerate_and_save_full_world() {
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
                write_rle_data(biomeBuffer, biomeAsChar);
                std::string biomeData = biomeBuffer.str();

                uint32_t biomeReserve = biomeData.size() + reserveSizeBiome;
                out.write(biomeData.data(), biomeData.size());
                out.write(std::string(biomeReserve - biomeData.size(), '~').c_str(), biomeReserve - biomeData.size());

                std::vector<std::vector<char>> objectAsChar(chunkSize, std::vector<char>(chunkSize));
                for (int y = 0; y < chunkSize; ++y)
                    for (int x = 0; x < chunkSize; ++x)
                        objectAsChar[y][x] = objectToSymbol(chunk.objectTiles[y][x]);

                std::stringstream objectBuffer;
                write_rle_data(objectBuffer, objectAsChar);
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

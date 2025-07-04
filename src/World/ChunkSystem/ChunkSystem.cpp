#include "ChunkSystem.h"
#include <sstream>
#include "Constants/WorldConst.h"
#include "Utilities/Logger/Logger.h"
#include "Constants/TilemapConst.h"

ChunkSystem::ChunkSystem(
    std::unordered_map<std::pair<int, int>, Chunk, PairHash>& chunks,
    const std::string& filename) : chunks(chunks), filename(filename) 
{
    LoadHeaders();
}

ChunkSystem::~ChunkSystem() {
    for (const auto& [coord, chunk] : chunks) {
        if (chunk.isModified) {
            int cx = coord.first;
            int cy = coord.second;
            overwriteChunk(cx, cy, chunk);
        }
    }
}

void ChunkSystem::LoadHeaders() {
    worldFile.open(filename + "/world.dat", std::ios::binary);
    if (!worldFile.is_open()) {
        mycerr << "can't open world.dat";
        return;
    }

    int totalChunks = worldSize;
    headers.resize(totalChunks);
    worldFile.read(reinterpret_cast<char*>(headers.data()), totalChunks * sizeof(ChunkHeader));
}

void ChunkSystem::update(Vector2& playerPos) {
    updateChunks(playerPos);
}

void ChunkSystem::render(Renderer& renderer) const {
    Texture2D& tilemap = renderer.getTexture("worldTilemap");
    for (auto& [key, chunk] : chunks) {
        chunk.DrawTiles(tilemap);
    }
    for (auto& [key, chunk] : chunks) {
        chunk.DrawObjects(tilemap);
    }
}

void ChunkSystem::updateChunks(Vector2& playerPos) {
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
                overwriteChunk(chunkX, chunkY, it->second);
                // it->second.isModified = false; anyway delete
            }
            it = chunks.erase(it);
        } 
        else {
            ++it;
        }
    }
}

void ChunkSystem::overwriteChunk(int cx, int cy, const Chunk& chunk) {
    std::fstream file(filename + "/world.dat", std::ios::binary | std::ios::in | std::ios::out);
    if (!file.is_open()) {
        mycerr << "Failed to open world.dat for overwriting chunk";
        return;
    }

    int index = cy * numberOfChunks + cx;
    file.seekg(index * sizeof(ChunkHeader), std::ios::beg);

    ChunkHeader header{};
    file.read(reinterpret_cast<char*>(&header), sizeof(ChunkHeader));

    if (file.fail()) {
        mycerr << "Failed to read ChunkHeader at index:" << index;
        file.close();
        return;
    }

    std::vector<std::vector<uint8_t>> biomeIds(chunkSize, std::vector<uint8_t>(chunkSize));
    for (int y = 0; y < chunkSize; ++y) {
        for (int x = 0; x < chunkSize; ++x) {
            uint8_t tileIndex = chunk.tiles[y][x];
            bool found = false;
            for (const auto& biome : BIOMES) {
                for (const auto& [tile, _] : biome.tileVariants) {
                    if (tile == tileIndex) {
                        biomeIds[y][x] = biome.id;
                        found = true;
                        break;
                    }
                }
                if (found) break;
            }
            if (!found) {
                biomeIds[y][x] = BIOMES[0].id;
                mycerr << "Unknown tileIndex while saving chunk (" << cx << "," << cy << "): " << (int)tileIndex;
            }
        }
    }

    std::vector<std::vector<uint8_t>> objectTypes(chunkSize, std::vector<uint8_t>(chunkSize, OBJECT_NONE));

    for (const Object& obj : chunk.objects) {
        auto it = objectPropertiesMap.find(obj.type);
        if (it == objectPropertiesMap.end()) continue;

        Vector2 size = it->second.size;
        int w = static_cast<int>(size.x);
        int h = static_cast<int>(size.y);

        int objGlobalX = static_cast<int>(obj.position.x / worldTileSize);
        int objGlobalY = static_cast<int>(obj.position.y / worldTileSize);

        int chunkStartX = cx * chunkSize;
        int chunkStartY = cy * chunkSize;
        int chunkEndX = chunkStartX + chunkSize;
        int chunkEndY = chunkStartY + chunkSize;

        if (objGlobalX + w <= chunkStartX || objGlobalX >= chunkEndX ||
            objGlobalY + h <= chunkStartY || objGlobalY >= chunkEndY)
            continue;

        int startX = std::max(objGlobalX, chunkStartX) - chunkStartX;
        int startY = std::max(objGlobalY, chunkStartY) - chunkStartY;

        int offsetX = std::max(chunkStartX - objGlobalX, 0);
        int offsetY = std::max(chunkStartY - objGlobalY, 0);

        int drawW = std::min(w - offsetX, chunkSize - startX);
        int drawH = std::min(h - offsetY, chunkSize - startY);

        for (int oy = 0; oy < drawH; ++oy) {
            int ty = startY + oy;
            for (int ox = 0; ox < drawW; ++ox) {
                int tx = startX + ox;
                objectTypes[ty][tx] = static_cast<uint8_t>(obj.type);
            }
        }
    }

    std::stringstream biomeBuffer;
    writeData(biomeBuffer, biomeIds);
    std::string biomeData = biomeBuffer.str();

    if (biomeData.size() > header.reservedSizeBiome) {
        mycerr << "Biome RLE data exceeds reserved space at chunk (" << cx << "," << cy << ")";
        saveFullWorld();
        file.close();
        return;
    }

    std::stringstream objectBuffer;
    writeObjectsChunk(objectBuffer, chunk.objects, cx * chunkSize, cy * chunkSize, worldTileSize);
    std::string objectData = objectBuffer.str();

    if (objectData.size() > header.reservedSizeObject) {
        mycerr << "Object data exceeds reserved space at chunk (" << cx << "," << cy << ")";
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

void ChunkSystem::writeData(std::ostream& out, const std::vector<std::vector<uint8_t>>& data) {
    uint8_t current = data[0][0];
    uint8_t count = 1;

    for (int y = 0; y < chunkSize; ++y) {
        for (int x = 0; x < chunkSize; ++x) {
            if (x == 0 && y == 0) continue;
            uint8_t next = data[y][x];
            if (next == current && count < 255) {
                count++;
            } else {
                out.write(reinterpret_cast<const char*>(&count), 1);
                out.write(reinterpret_cast<const char*>(&current), 1);
                current = next;
                count = 1;
            }
        }
    }

    out.write(reinterpret_cast<const char*>(&count), 1);
    out.write(reinterpret_cast<const char*>(&current), 1);
}

void ChunkSystem::saveFullWorld() {
    std::string oldFile = filename + "/world.dat";
    std::string newFile = filename + "/world_temp.dat";

    std::ifstream in(oldFile, std::ios::binary);
    std::ofstream out(newFile, std::ios::binary | std::ios::trunc);

    if (!in.is_open() || !out.is_open()) {
        mycerr << "Failed to open world file";
        return;
    }

    int total_chunks = numberOfChunks * numberOfChunks;
    std::vector<ChunkHeader> oldHeaders(total_chunks);
    std::vector<ChunkHeader> newHeaders(total_chunks);

    in.read(reinterpret_cast<char*>(oldHeaders.data()), total_chunks * sizeof(ChunkHeader));
    if (in.gcount() != total_chunks * sizeof(ChunkHeader)) {
        mycerr << "Corrupted or incomplete header section";
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

                std::vector<std::vector<uint8_t>> biomeIds(chunkSize, std::vector<uint8_t>(chunkSize));
                for (int y = 0; y < chunkSize; ++y) {
                    for (int x = 0; x < chunkSize; ++x) {
                        uint8_t tileIndex = chunk.tiles[y][x];
                        bool found = false;
                        for (const auto& biome : BIOMES) {
                            for (const auto& [tile, _] : biome.tileVariants) {
                                if (tile == tileIndex) {
                                    biomeIds[y][x] = biome.id;
                                    found = true;
                                    break;
                                }
                            }
                            if (found) break;
                        }
                        if (!found) {
                            biomeIds[y][x] = BIOMES[0].id;
                            mycerr << "Unknown tileIndex while saving chunk (" << cx << "," << cy << "): " << (int)tileIndex;
                        }
                    }
                }

                std::stringstream biomeBuffer;
                writeData(biomeBuffer, biomeIds);
                std::string biomeData = biomeBuffer.str();

                uint32_t biomeDataSize = biomeData.size();
                uint32_t biomeReserve = biomeDataSize + reserveSizeBiome;

                std::string biomePadding(biomeReserve - biomeDataSize, '~');
                out.write(biomeData.data(), biomeDataSize);
                out.write(biomePadding.data(), biomePadding.size());

                std::stringstream objectBuffer;
                writeObjectsChunk(objectBuffer, chunk.objects, cx * chunkSize, cy * chunkSize, worldTileSize);
                std::string objectData = objectBuffer.str();

                uint32_t objectDataSize = static_cast<uint32_t>(objectData.size());
                uint32_t objectReserve = objectDataSize + reserveSizeObject;

                std::string objectPadding(objectReserve - objectDataSize, '~');
                out.write(objectData.data(), objectDataSize);
                out.write(objectPadding.data(), objectPadding.size());

                newHeaders[index] = {
                    static_cast<uint32_t>(biomeOffset),
                    biomeDataSize,
                    biomeReserve,
                    static_cast<uint32_t>(biomeOffset) + static_cast<uint32_t>(biomeReserve),
                    objectDataSize,
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

    in.close();
    out.close();

    std::filesystem::copy_file(newFile, oldFile, std::filesystem::copy_options::overwrite_existing);
    std::filesystem::remove(newFile);
}

void ChunkSystem::writeObjectsChunk(std::ostream& out, const std::vector<Object>& objects, int startTileX, int startTileY, int tileSize) {
    uint16_t count = static_cast<uint16_t>(objects.size());
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const Object& obj : objects) {
        FileObject fobj;
        fobj.type = static_cast<uint8_t>(obj.type);

        int tileX = static_cast<int>(obj.position.x / tileSize) - startTileX;
        int tileY = static_cast<int>(obj.position.y / tileSize) - startTileY;

        fobj.localX = static_cast<uint8_t>(tileX);
        fobj.localY = static_cast<uint8_t>(tileY);

        out.write(reinterpret_cast<const char*>(&fobj), sizeof(fobj));
    }
}
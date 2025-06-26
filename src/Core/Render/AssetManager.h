#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "raylib.h"
#include <string>
#include <unordered_map>

class AssetManager {
public:
    AssetManager();
    ~AssetManager();

    void registerTexture(const std::string& key, const std::string& path);
    void registerFont(const std::string& key, const std::string& path);
    void registerSound(const std::string& key, const std::string& path);

    Texture2D& getTexture(const std::string& key, bool shouldBeWrapped = false);
    Font& getFont(const std::string& key, int size);
    Sound& getSound(const std::string& key);

    void unloadAll();

private:
    std::unordered_map<std::string, std::string> texturePaths;
    std::unordered_map<std::string, std::string> fontPaths;
    std::unordered_map<std::string, std::string> soundPaths;

    std::unordered_map<std::string, Texture2D> textures;
    std::unordered_map<std::string, std::unordered_map<int, Font>> fonts;
    std::unordered_map<std::string, Sound> sounds;
};

#endif //ASSETMANAGER_H

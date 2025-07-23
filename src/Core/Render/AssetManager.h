#pragma once
#include <unordered_map>
#include <map>
#include <string>
#include <filesystem>
#include "raylib.h"

class AssetManager {
public:
    AssetManager();  // to register every asset
    void init();
    ~AssetManager();
    void unloadAll(); 

    Texture2D& getTexture(const std::string& key, bool shouldBeWrapped = false);
    Font& getFont(const std::string& key, int size);
    Sound& getSound(const std::string& key);
    
private:
    template<typename MapType>
    void tryRegisterPath(MapType& map, const std::string& key, const std::string& path);
    
    std::unordered_map<std::string, std::string> texturePaths;  // file name to name map
    std::unordered_map<std::string, std::string> fontPaths;     // file name to name map
    std::unordered_map<std::string, std::string> soundPaths;    // file name to name map
    
    std::unordered_map<std::string, Texture2D> textures;        // name to asset map
    std::unordered_map<std::string, std::map<int, Font>> fonts; // name to asset map
    std::unordered_map<std::string, Sound> sounds;              // name to asset map
    
    Texture2D defaultTexture{};
    Font defaultFont{};
    Sound defaultSound{};

    void registerTexture(const std::string& key, const std::string& path);
    void registerFont(const std::string& key, const std::string& path);
    void registerSound(const std::string& key, const std::string& path);
};
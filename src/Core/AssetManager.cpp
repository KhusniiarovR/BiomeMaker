#include "AssetManager.h"
#include <iostream>

AssetManager::AssetManager() {
    registerFont("Inter", "data/text/InterFont.ttf");
}

AssetManager::~AssetManager() {
    unloadAll();
}

void AssetManager::registerTexture(const std::string& key, const std::string& path) {
        if (texturePaths.contains(key)) {
            std::cerr << "AssetManager: Texture key already registered: " << key << "\n";
            return;
        }
        texturePaths[key] = path;
    }

void AssetManager::registerFont(const std::string& key, const std::string& path) {
    if (fontPaths.contains(key)) {
        std::cerr << "AssetManager: Font key already registered: " << key << "\n";
        return;
    }
    fontPaths[key] = path;
}

void AssetManager::registerSound(const std::string& key, const std::string& path) {
    if (soundPaths.contains(key)) {
        std::cerr << "AssetManager: Sound key already registered: " << key << "\n";
        return;
    }
    soundPaths[key] = path;
}

Texture2D& AssetManager::getTexture(const std::string& key) {
    auto it = textures.find(key);
    if (it != textures.end()) return it->second;

    auto pathIt = texturePaths.find(key);
    if (pathIt == texturePaths.end()) {
        std::cerr << "AssetManager: Texture key not found: " << key << "\n";
        static Texture2D texture;
        return texture;
    }

    Texture2D tex = LoadTexture(pathIt->second.c_str());
    textures[key] = tex;
    return textures[key];
}

Font& AssetManager::getFont(const std::string& key, int size) {
    auto it = fonts.find(key);
    if (it != fonts.end()) {
        auto sizeIt = it->second.find(size);
        if (sizeIt != it->second.end()) return sizeIt->second;
    }

    auto pathIt = fontPaths.find(key);
    if (pathIt == fontPaths.end()) {
        std::cerr << "AssetManager: Font key not found: " << key << "\n";
        static Font font;
        return font;
    }

    Font font = LoadFontEx(pathIt->second.c_str(), size, nullptr, 0);
    fonts[key][size] = font;
    return fonts[key][size];;
}

Sound& AssetManager::getSound(const std::string& key) {
    auto it = sounds.find(key);
    if (it != sounds.end()) return it->second;

    auto pathIt = soundPaths.find(key);
    if (pathIt == soundPaths.end()) {
        std::cerr << "AssetManager: Sound key not found: " << key << "\n";
        static Sound sound;
        return sound;
    }

    Sound sound = LoadSound(pathIt->second.c_str());
    sounds[key] = sound;
    return sounds[key];
}

void AssetManager::unloadAll() {
    for (auto& [key, tex] : textures) {
        UnloadTexture(tex);
    }
    textures.clear();

    for (auto& [key, sizeMap] : fonts) {
        for (auto& [size, font] : sizeMap) {
            UnloadFont(font);
        }
    }
    fonts.clear();

    for (auto& [key, sound] : sounds) {
        UnloadSound(sound);
    }
    sounds.clear();
}

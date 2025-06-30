#include "AssetManager.h"
#include "Utilities/Logger/Logger.h"
#include "Items/ItemRegister.h"

AssetManager::AssetManager() {
    registerFont("inter", "data/text/InterFont.ttf");
    registerTexture("worldTilemap", "data/textures/worldTilemap.png");
    registerTexture("button1", "data/textures/buttonPack1.png");
    registerTexture("background", "data/textures/background.png");
    registerTexture("itemTilemap", "data/textures/itemTilemap.png");
    registerTexture("entityTilemap", "data/textures/entityTilemap.png");
    registerAllItems();
}

AssetManager::~AssetManager() {
    unloadAll();
}

void AssetManager::registerTexture(const std::string& key, const std::string& path) {
    if (texturePaths.contains(key)) {
        mycerr << "Texture key already registered: " << key;
        return;
    }
    texturePaths[key] = path;
}

void AssetManager::registerFont(const std::string& key, const std::string& path) {
    if (fontPaths.contains(key)) {
        mycerr << "Font key already registered: " << key;
        return;
    }
    fontPaths[key] = path;
}

void AssetManager::registerSound(const std::string& key, const std::string& path) {
    if (soundPaths.contains(key)) {
        mycerr << "Sound key already registered: " << key;
        return;
    }
    soundPaths[key] = path;
}

Texture2D& AssetManager::getTexture(const std::string& key, bool shouldBeWrapped) {
    auto it = textures.find(key);
    if (it != textures.end()) return it->second;

    auto pathIt = texturePaths.find(key);
    if (pathIt == texturePaths.end()) {
        mycerr << "Texture key not found: " << key;
        static Texture2D texture;
        return texture;
    }

    Texture2D tex = LoadTexture(pathIt->second.c_str());
    
    if (tex.id == 0) {
        mycerr << "incorrect texture path: " << key;
    }
    
    SetTextureFilter(tex, TEXTURE_FILTER_POINT);
    if (shouldBeWrapped) SetTextureWrap(tex, TEXTURE_WRAP_REPEAT);

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
        mycerr << "Font key not found: " << key;
        static Font font;
        return font;
    }

    Font font = LoadFontEx(pathIt->second.c_str(), size, nullptr, 0);

    if (font.texture.id == 0) {
        mycerr << "incorrect font path: " << key;
    }

    SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);

    fonts[key][size] = font;
    return fonts[key][size];;
}

Sound& AssetManager::getSound(const std::string& key) {
    auto it = sounds.find(key);
    if (it != sounds.end()) return it->second;

    auto pathIt = soundPaths.find(key);
    if (pathIt == soundPaths.end()) {
        mycerr << "Sound key not found: " << key;
        static Sound sound;
        return sound;
    }

    Sound sound = LoadSound(pathIt->second.c_str());

    if (sound.frameCount == 0) {
        mycerr << "incorrect sound path: " << key;
    }

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

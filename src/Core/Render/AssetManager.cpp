#include "AssetManager.h"
#include "Utilities/Logger/Logger.h"
#include "Items/ItemRegister/ItemRegisterAll.h"
#include <filesystem>

AssetManager::AssetManager() // register all assets
{
    // fonts
    registerFont("inter", "data/Fonts/InterFont.ttf");
    registerFont("silkscreen", "data/Fonts/Silkscreen-Regular.ttf");

    // textures
    registerTexture("background",   "data/Textures/Menu/background.png"); // menu scrolling background
    registerTexture("button1",      "data/Textures/Ui/buttonPack1.png");  // button states
    registerTexture("worldTilemap", "data/Textures/World/worldTilemap.png"); // biome and objects
    registerTexture("itemTilemap",  "data/Textures/World/itemTilemap.png"); // items
    registerTexture("entityTilemap","data/Textures/World/entityTilemap.png"); // entities
    registerTexture("playerTilemap","data/Textures/World/playerTilemap.png"); // player

    registerAllItems(); // register all items
}

AssetManager::~AssetManager() 
{
    unloadAll();
    UnloadTexture(defaultTexture);
    UnloadFont(defaultFont);
    UnloadSound(defaultSound);
}

void AssetManager::init()
{
    // default assets if something is missing
    defaultTexture = LoadTexture("data/Default/missingTexture.png");
    defaultFont =    LoadFont   ("data/Fonts/InterFont.ttf");
    defaultSound =   LoadSound  ("data/Default/missingSound.wav");
}

template<typename MapType>
void AssetManager::tryRegisterPath(MapType& map, const std::string& key, const std::string& path) 
{
    if (map.find(key) != map.end()) 
    {
        mycerr << "Key already registered: " << key;
        return;
    }
    map[key] = path;
}

void AssetManager::registerTexture(const std::string& key, const std::string& path) 
{
    tryRegisterPath(texturePaths, key, path);
}

void AssetManager::registerFont(const std::string& key, const std::string& path) 
{
    tryRegisterPath(fontPaths, key, path);
}

void AssetManager::registerSound(const std::string& key, const std::string& path) 
{
    tryRegisterPath(soundPaths, key, path);
}

Texture2D& AssetManager::getTexture(const std::string& key, bool shouldBeWrapped) // lazy getting textures
{
    auto it = textures.find(key);
    if (it != textures.end()) return it->second;

    auto pathIt = texturePaths.find(key);
    if (pathIt == texturePaths.end()) 
    {
        mycerr << "Texture key not found: " << key;
        return defaultTexture;
    }

    if (!std::filesystem::exists(pathIt->second))
    {
        mycerr << "Texture file not found: " << pathIt->second;
        return defaultTexture;
    }

    Texture2D texture = LoadTexture(pathIt->second.c_str());
    if (texture.id == 0) 
    {
        mycerr << "Failed to load texture: " << key;
        return defaultTexture;
    }

    SetTextureFilter(texture, TEXTURE_FILTER_POINT);
    if (shouldBeWrapped) SetTextureWrap(texture, TEXTURE_WRAP_REPEAT);

    auto [itInserted, _] = textures.emplace(key, texture);
    return itInserted->second;
}

Font& AssetManager::getFont(const std::string& key, int size) // lazy getting sounds
{
    auto it = fonts.find(key);
    if (it != fonts.end())
    {
        auto sizeIt = it->second.find(size);
        if (sizeIt != it->second.end()) return sizeIt->second;
    }

    auto pathIt = fontPaths.find(key);
    if (pathIt == fontPaths.end()) 
    {
        mycerr << "Font key not found: " << key;
        return defaultFont;
    }

    if (!std::filesystem::exists(pathIt->second)) 
    {
        mycerr << "Font file not found: " << pathIt->second;
        return defaultFont;
    }

    Font font = LoadFontEx(pathIt->second.c_str(), size, nullptr, 0);
    if (font.texture.id == 0) 
    {
        mycerr << "Failed to load font: " << key;
        return defaultFont;
    }

    SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);

    auto& sizeMap = fonts[key];
    auto [sizeIt, _] = sizeMap.emplace(size, font);
    return sizeIt->second;
}

Sound& AssetManager::getSound(const std::string& key) // lazy getting textures
{
    auto it = sounds.find(key);
    if (it != sounds.end()) return it->second;

    auto pathIt = soundPaths.find(key);
    if (pathIt == soundPaths.end()) 
    {
        mycerr << "Sound key not found: " << key;
        return defaultSound;
    }

    if (!std::filesystem::exists(pathIt->second)) 
    {
        mycerr << "Sound file not found: " << pathIt->second;
        return defaultSound;
    }

    Sound sound = LoadSound(pathIt->second.c_str());
    if (sound.frameCount == 0) 
    {
        mycerr << "Failed to load sound: " << key;
        return defaultSound;
    }

    auto [itInserted, _] = sounds.emplace(key, sound);
    return itInserted->second;
}

void AssetManager::unloadAll() // unload every asset
{
    for (auto& [_, tex] : textures) { UnloadTexture(tex); }
    textures.clear();

    for (auto& [_, sizeMap] : fonts) {
        for (auto& [_, font] : sizeMap) { UnloadFont(font); }
    }
    fonts.clear();

    for (auto& [_, sound] : sounds) { UnloadSound(sound); }
    sounds.clear();
}

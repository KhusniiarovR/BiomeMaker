#include "Settings.h"
#include <fstream>
#include "Utilities/Input/KeysTranslate.h"
#include "SettingsConstants.h"
#include "Core/Render/AssetManager.h"
#include "Notifications/NotificationManager.h"

Settings::Settings(const std::string& path) 
: configPath(path),
defaultButton({0.7f, 0.4f}, {0.2f, 0.1f}, "default", BLACK, 0.5f),
applyButton  ({0.7f, 0.6f}, {0.2f, 0.1f}, "apply", BLACK, 0.5f),
quitButton   ({0.7f, 0.8f}, {0.2f, 0.1f}, "quit", BLACK, 0.5f)
{
    init();
    registerHandlers();
    load();
    apply();
}

void Settings::init() 
{
    // buttons functionality
    defaultButton.setOnClick([this]() {
        loadDefault();
        NotificationManager::getInstance().show("default settings loaded", NotificationType::Warning, 4.0f);
    });
    applyButton.setOnClick([this]() {
        save();
        NotificationManager::getInstance().show("saved", NotificationType::Info, 4.0f);
    });
    quitButton.setOnClick([this]() { settingsActive = false; });

    // buttons textures
    defaultButton.setTexture(AssetManager::instance().getTexture("button1"));
    applyButton.setTexture(AssetManager::instance().getTexture("button1"));
    quitButton.setTexture(AssetManager::instance().getTexture("button1"));
}

void Settings::load()
{
    std::ifstream file(configPath);
    if (!file.is_open())
    {
        loadDefault();
        save();
        return;
    }
    loadFromFile();
}

void Settings::save() 
{
    std::ofstream file(configPath);
    file << "#screen\n";
    file << "window_width=" << data.windowWidth << "\n";
    file << "window_height=" << data.windowHeight << "\n\n";

    file << "#fps\n";
    file << "max_fps=" << data.maxFPS << "\n\n";

    file << "#keybinds\n";
    for (int i = 0; i < static_cast<int>(Action::ActionSize); i++) 
    {
        Action action = static_cast<Action>(i);
        file << InputManager::GetNameFromAction(action) << "=" << GetNameFromKey(InputManager::GetInstance().GetKeyBind(action)) << "\n";
    }

    apply();
}

void Settings::loadFromFile()
{
    widgets.clear();

    std::ifstream file(configPath);
    if (!file.is_open()) 
    {
        mycerr << "Failed to open config file for reading: " << configPath;
        return;
    }

    std::string line;
    while (std::getline(file, line)) 
    {
        if (line.empty()) continue;

        static const std::unordered_map<std::string, std::string> sectionMap = {
            {"#screen", "Screen Settings"},
            {"#fps", "Performance"},
            {"#keybinds", "Key Bindings"}
        };

        if (sectionMap.count(line)) // finds all SectionHeaders
        {
            widgets.push_back(std::make_unique<SectionHeader>(sectionMap.at(line)));
            continue;
        }

        // then tries to fing key=value line
        size_t eq = line.find('=');
        if (eq == std::string::npos) { continue; }

        std::string key = line.substr(0, eq);
        std::string val = line.substr(eq + 1);

        if (val.empty()) {
            mycerr << "Warning: empty value for key '" << key << "'\n";
            continue;
        }

        auto handlerIt = settingHandlers.find(key);
        if (handlerIt != settingHandlers.end()) { handlerIt->second(val); } // for int widgets
        else // for key bing widgets
        {
            Action action = InputManager::GetActionFromName(key.c_str());
            if (action != Action::ActionSize) 
            {
                int keyCode = GetKeyFromName(val.c_str());
                InputManager::GetInstance().SetKeyBind(action, keyCode);
                widgets.push_back(std::make_unique<KeyBindField>(action));
            }
        }
    }
}

void Settings::update(float dt, Vector2 mouseVirtual) 
{
    // buttons
    defaultButton.update(mouseVirtual);
    applyButton.update(mouseVirtual);
    quitButton.update(mouseVirtual);

    // scroll
    if (IsKeyDown(KEY_DOWN)) scrollOffset += SCROLL_SPEED;
    if (IsKeyDown(KEY_UP)) scrollOffset -= SCROLL_SPEED;
    float wheel = GetMouseWheelMove();
    scrollOffset -= wheel * SCROLL_SPEED;
    scrollOffset = std::max(scrollOffset, 0.0f);

    // total settings Y size
    int totalHeight = INITIAL_Y;
    for (const auto& widget : widgets) {
        totalHeight += widget->getHeight();
    }

    // bound scroll
    int maxScroll = std::max(0, totalHeight - virtualScreenSizeY);
    scrollOffset = std::min(scrollOffset, (float)maxScroll);

    // widgets
    int y = INITIAL_Y;
    for (auto& widget : widgets) 
    {
        widget->update(mouseVirtual, y - scrollOffset);
        y += widget->getHeight();
    }
}

void Settings::render(Renderer& renderer) const 
{
    BeginScissorMode(0, 0, virtualScreenSizeX, virtualScreenSizeY); // to cut off things that are outside of the screen
    DrawRectangle(0, 0, virtualScreenSizeX, virtualScreenSizeY, Fade(BLACK, 0.5f)); // make everything that is not settings darker

    // buttons
    defaultButton.render(renderer);
    applyButton.render(renderer);
    quitButton.render(renderer);

    // widgets
    int y = INITIAL_Y;
    for (const auto& widget : widgets) 
    {
        widget->render(renderer, y - scrollOffset);
        y += widget->getHeight();
    }

    EndScissorMode();
}

void Settings::registerHandlers() // for int fields
{
    settingHandlers["window_width"] = [&](const std::string& val) 
    {
        data.windowWidth = std::stoi(val);
        widgets.push_back(std::make_unique<IntField>("Window Width", &data.windowWidth, 640, GetMonitorWidth(0)));
    };

    settingHandlers["window_height"] = [&](const std::string& val) 
    {
        data.windowHeight = std::stoi(val);
        widgets.push_back(std::make_unique<IntField>("Window Height", &data.windowHeight, 480, GetMonitorHeight(0)));
    };

    settingHandlers["max_fps"] = [&](const std::string& val) 
    {
        data.maxFPS = std::stoi(val);
        widgets.push_back(std::make_unique<IntField>("Max FPS", &data.maxFPS, 24, 1000));
    };
}

void Settings::apply() const
{
    SetWindowSize(data.windowWidth, data.windowHeight);
    SetTargetFPS(data.maxFPS);
}

void Settings::loadDefault() 
{
    data = SettingsData();
    InputManager::GetInstance().LoadDefaultBinds();
}
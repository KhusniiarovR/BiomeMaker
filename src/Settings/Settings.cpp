#include "Settings.h"
#include <fstream>
#include "Utilities/Input/KeysTranslate.h"
#include "Utilities/Logger/Logger.h"
#include <algorithm>
#include "SettingsConstants.h"

Settings::Settings(const std::string& path) : configPath(path) {
    Load();
}

Settings::~Settings() {
    Save();
}

void Settings::Load() {
    sections.clear();
    actionKeyList.clear();
    
    std::ifstream file(configPath);
    std::string line;
    int lineNumber = 0;
    
    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty()) continue;
        
        if (line[0] == '#') {
            ConfigSection section;
            section.title = line;
            section.startLine = lineNumber;
            sections.push_back(section);
            continue;
        }
        
        ParseLine(line, lineNumber);
    }
    
    // Initialize action list from InputManager
    auto& input = InputManager::GetInstance();
    for (int i = 0; i < static_cast<int>(Action::COUNT); i++) {
        Action action = static_cast<Action>(i);
        actionKeyList.emplace_back(action, input.GetKeyBinding(action));
    }
}

void Settings::ParseLine(const std::string& line, int lineNumber) {
    size_t eq = line.find('=');
    if (eq == std::string::npos) return;
    
    std::string key = line.substr(0, eq);
    std::string val = line.substr(eq + 1);
    
    if (key == "window_width") {
        windowWidth = std::stoi(val);
    }
    else if (key == "window_height") {
        windowHeight = std::stoi(val);
    }
    else if (key == "max_fps") {
        maxFPS = std::stoi(val);
    }
    else {
        Action action = InputManager::GetActionFromName(key.c_str());
        if (action != Action::COUNT) {
            int keyCode = GetKeyFromName(val.c_str());
            if (keyCode > 0) {
                InputManager::GetInstance().SetKeyBinding(action, keyCode);
            }
        }
    }
}

void Settings::Save() {
    std::ofstream file(configPath);
    
    // Write sections
    file << "#screen\n";
    file << "window_width=" << windowWidth << "\n";
    file << "window_height=" << windowHeight << "\n\n";
    
    file << "#fps\n";
    file << "max_fps=" << maxFPS << "\n\n";
    
    file << "#keybinds\n";
    auto& input = InputManager::GetInstance();
    for (int i = 0; i < static_cast<int>(Action::COUNT); i++) {
        Action action = static_cast<Action>(i);
        file << InputManager::GetActionName(action) << "=" 
             << GetKeyName(input.GetKeyBinding(action)) << "\n";
    }

    SetWindowSize(windowWidth, windowHeight);
    SetTargetFPS(maxFPS);
}

void Settings::Update(float dt, Vector2 mouseVirtual) {
    float wheel = GetMouseWheelMove();
    scrollOffset -= wheel * SCROLL_SPEED;
    scrollOffset = std::max(scrollOffset, 0.0f);
    
    int totalHeight = CalculateTotalHeight();
    int maxScroll = std::max(0, totalHeight - virtualScreenSizeY);
    scrollOffset = std::min(scrollOffset, (float)maxScroll);

    if (waitingForKey) {
        for (int k = 32; k < 349; ++k) {
            if (IsKeyPressed(k)) {
                InputManager::GetInstance().SetKeyBinding(currentBinding, k);
                for (auto& pair : actionKeyList) {
                    if (pair.first == currentBinding) {
                        pair.second = k;
                        break;
                    }
                }
                waitingForKey = false;
                break;
            }
        }
        return;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !editingField.empty()) {
        bool clickedOnField = false;
        
        if (!clickedOnField) {
            editingField = "";
            inputBuffer = "";
        }
    }

    for (size_t i = 0; i < sections.size(); ++i) {
        int sectionY = GetSectionContentYPosition(i) - static_cast<int>(scrollOffset);
        const auto& section = sections[i];
        
        if (section.title.find("#screen") != std::string::npos) {
            UpdateIntField("windowWidth", windowWidth, virtualScreenSizeX, 9999, 
                         GetSectionContentYPosition(i), mouseVirtual);
            
            UpdateIntField("windowHeight", windowHeight, virtualScreenSizeY, 9999, 
                         GetSectionContentYPosition(i) + LINE_HEIGHT, mouseVirtual);
        }
        else if (section.title.find("#fps") != std::string::npos) {
            UpdateIntField("maxFPS", maxFPS, 30, 1000, 
                         GetSectionContentYPosition(i), mouseVirtual);
        }
        else if (section.title.find("#keybinds") != std::string::npos) {
            int keybindY = GetSectionContentYPosition(i) - static_cast<int>(scrollOffset);
            for (const auto& [action, _] : actionKeyList) {
                if (keybindY >= 0 && keybindY <= virtualScreenSizeY - FIELD_HEIGHT) {
                    Rectangle btn = {LABEL_WIDTH, (float)keybindY, 100, FIELD_HEIGHT};
                    if (CheckCollisionPointRec(mouseVirtual, btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        waitingForKey = true;
                        currentBinding = action;
                        break;
                    }
                }
                keybindY += LINE_HEIGHT;
            }
        }
    }

    if (IsKeyPressed(KEY_Q)) {
        Save();
    }
}

void Settings::Render() const {
    BeginScissorMode(0, 0, virtualScreenSizeX, virtualScreenSizeY);

    int y = INITIAL_Y - static_cast<int>(scrollOffset);
    
    for (size_t i = 0; i < sections.size(); ++i) {
        int sectionY = GetSectionStartYPosition(i) - static_cast<int>(scrollOffset);
        if (sectionY + SECTION_TITLE_SIZE >= 0 && sectionY <= virtualScreenSizeY) {
            const auto& section = sections[i];
            
            if (section.title.find("#screen") != std::string::npos) {
                DrawSectionTitle("Screen Settings", sectionY);
                sectionY = GetSectionContentYPosition(i) - static_cast<int>(scrollOffset);
                
                RenderIntField("Window Width", "windowWidth", windowWidth, sectionY); 
                sectionY += LINE_HEIGHT;
                RenderIntField("Window Height", "windowHeight", windowHeight, sectionY);
            }
            else if (section.title.find("#fps") != std::string::npos) {
                DrawSectionTitle("Performance", sectionY);
                sectionY = GetSectionContentYPosition(i) - static_cast<int>(scrollOffset);
                
                RenderIntField("Max FPS", "maxFPS", maxFPS, sectionY);
            }
            else if (section.title.find("#keybinds") != std::string::npos) {
                DrawSectionTitle("Key Bindings", sectionY);
                sectionY = GetSectionContentYPosition(i) - static_cast<int>(scrollOffset);
                
                for (const auto& [action, _] : actionKeyList) {
                    DrawKeyBind(action, sectionY);
                    sectionY += LINE_HEIGHT;
                }
            }
        }
    }

    EndScissorMode();
}

void Settings::DrawSectionTitle(const std::string& title, int& y) const {
    DrawText(title.c_str(), 20, y, SECTION_TITLE_SIZE, COLOR_SECTION_TITLE);
    y += LINE_HEIGHT;
}

void Settings::DrawKeyBind(Action action, int y) const {
    DrawText(InputManager::GetActionName(action), 20, y, TEXT_SIZE, COLOR_TEXT);

    Rectangle btn = {LABEL_WIDTH, (float)y, 100, FIELD_HEIGHT};
    DrawRectangleRec(btn, COLOR_KEYBIND_BTN);

    std::string keyName = (waitingForKey && currentBinding == action) ? "..." : 
        GetKeyName(InputManager::GetInstance().GetKeyBinding(action));

    DrawText(keyName.c_str(), btn.x + 10, btn.y + 5, TEXT_SIZE, COLOR_TEXT);
}

bool Settings::IsEditingField(const std::string& name) const {
    return editingField == name;
}

void Settings::UpdateIntField(const std::string& fieldName, int& value, int min, int max, int y, Vector2 mouseVirtual) {
    float visibleY = y - scrollOffset;
    
    if (visibleY + FIELD_HEIGHT < 0 || visibleY > virtualScreenSizeY) {
        return;
    }
    Rectangle fieldRect = {LABEL_WIDTH, visibleY, FIELD_WIDTH, FIELD_HEIGHT};

    if (CheckCollisionPointRec(mouseVirtual, fieldRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        editingField = fieldName;
        inputBuffer = std::to_string(value);
    }

    if (IsEditingField(fieldName)) {
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= '0' && key <= '9' && inputBuffer.size() < 6) {
                inputBuffer += static_cast<char>(key);
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !inputBuffer.empty()) {
            inputBuffer.pop_back();
        }

        if (IsKeyPressed(KEY_ENTER)) {
            if (!inputBuffer.empty()) {
                int newValue = std::stoi(inputBuffer);
                value = std::clamp(newValue, min, max);
            }
            editingField = "";
            inputBuffer = "";
        }
    }

    Rectangle plusBtn = {LABEL_WIDTH + FIELD_WIDTH + BUTTON_SPACING, (float)y, BUTTON_WIDTH, FIELD_HEIGHT};
    plusBtn.y -= scrollOffset;
    Rectangle minusBtn = {LABEL_WIDTH + FIELD_WIDTH + BUTTON_SPACING * 2 + BUTTON_WIDTH, (float)y, BUTTON_WIDTH, FIELD_HEIGHT};
    minusBtn.y -= scrollOffset;

    if (CheckCollisionPointRec(mouseVirtual, plusBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        value = std::min(value + 10, max);
    }
    if (CheckCollisionPointRec(mouseVirtual, minusBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        value = std::max(value - 10, min);
    }
}

void Settings::RenderIntField(const char* label, const std::string& fieldName, int value, int y) const {
    DrawText(label, 20, y, TEXT_SIZE, COLOR_TEXT);

    Rectangle fieldRect = {LABEL_WIDTH, (float)y, FIELD_WIDTH, FIELD_HEIGHT};
    Color bgColor = IsEditingField(fieldName) ? COLOR_BG_EDITING : COLOR_BG_DEFAULT;
    DrawRectangleRec(fieldRect, bgColor);

    std::string displayText = IsEditingField(fieldName) ? inputBuffer : std::to_string(value);
    DrawText(displayText.c_str(), fieldRect.x + 10, fieldRect.y + 5, TEXT_SIZE, COLOR_TEXT_FIELD);

    DrawRectangle(fieldRect.x + FIELD_WIDTH + BUTTON_SPACING, y, BUTTON_WIDTH, FIELD_HEIGHT, COLOR_PLUS_MINUS_BTN);
    DrawRectangle(fieldRect.x + FIELD_WIDTH + BUTTON_SPACING * 2 + BUTTON_WIDTH, y, BUTTON_WIDTH, FIELD_HEIGHT, COLOR_PLUS_MINUS_BTN);
    DrawText("+", fieldRect.x + FIELD_WIDTH + BUTTON_SPACING + 8, y + 5, TEXT_SIZE, COLOR_TEXT);
    DrawText("-", fieldRect.x + FIELD_WIDTH + BUTTON_SPACING * 2 + BUTTON_WIDTH + 8, y + 5, TEXT_SIZE, COLOR_TEXT);
}

bool Settings::isActive() const {
    return settingsActive;
}

void Settings::setActive(bool active) {
    settingsActive = active;
}

int Settings::CalculateTotalHeight() const {
    if (sections.empty()) return INITIAL_Y;
    return GetSectionStartYPosition(sections.size() - 1) + 
           GetSectionHeight(sections.size() - 1);
}

int Settings::GetSectionHeight(int sectionIndex) const {
    const auto& section = sections[sectionIndex];
    if (section.title.find("#screen") != std::string::npos) {
        return SECTION_TITLE_SIZE + 2 * LINE_HEIGHT;
    }
    else if (section.title.find("#fps") != std::string::npos) {
        return SECTION_TITLE_SIZE + LINE_HEIGHT;
    }
    else if (section.title.find("#keybinds") != std::string::npos) {
        return SECTION_TITLE_SIZE + 
               static_cast<int>(actionKeyList.size()) * LINE_HEIGHT;
    }
    return 0;
}

int Settings::GetSectionContentYPosition(int sectionIndex) const {
    int y = GetSectionStartYPosition(sectionIndex);
    if (sectionIndex < sections.size()) {
        y += SECTION_TITLE_SIZE;
    }
    return y;
}

int Settings::GetSectionStartYPosition(int sectionIndex) const {
    int y = INITIAL_Y;
    
    for (int i = 0; i < sectionIndex; ++i) {
        if (sections[i].title.find("#screen") != std::string::npos) {
            y += SECTION_TITLE_SIZE + 2 * LINE_HEIGHT;
        }
        else if (sections[i].title.find("#fps") != std::string::npos) {
            y += SECTION_SPACING + SECTION_TITLE_SIZE + LINE_HEIGHT;
        }
        else if (sections[i].title.find("#keybinds") != std::string::npos) {
            y += SECTION_SPACING + SECTION_TITLE_SIZE + 
                static_cast<int>(actionKeyList.size()) * LINE_HEIGHT;
        }
    }
    
    return y;
}
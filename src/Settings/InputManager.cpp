#include "InputManager.h"
#include "Utilities/Input/KeysTranslate.h"

InputManager& InputManager::GetInstance() {
    static InputManager instance;
    return instance;
}

InputManager::InputManager() {
    LoadDefaultBindings();
}

void InputManager::LoadDefaultBindings() {
    keyBindings[Action::MOVE_LEFT] = KEY_A;
    keyBindings[Action::MOVE_RIGHT] = KEY_D;
    keyBindings[Action::MOVE_UP] = KEY_W;
    keyBindings[Action::MOVE_DOWN] = KEY_S;
    keyBindings[Action::INTERACT] = KEY_E;
    keyBindings[Action::INVENTORY] = KEY_TAB;
    keyBindings[Action::ENTER] = KEY_ENTER;
    keyBindings[Action::ESCAPE] = KEY_ESCAPE;
}

void InputManager::SetKeyBinding(Action action, int key) {
    keyBindings[action] = key;
}

int InputManager::GetKeyBinding(Action action) const {
    auto it = keyBindings.find(action);
    return it != keyBindings.end() ? it->second : 0;
}

bool InputManager::IsActionPressed(Action action) const {
    return IsKeyDown(GetKeyBinding(action));
}

bool InputManager::IsActionJustPressed(Action action) const {
    return IsKeyPressed(GetKeyBinding(action));
}

const char* InputManager::GetActionName(Action action) {
    static const char* names[] = {
        "move_left",
        "move_right",
        "move_up",
        "move_down",
        "interact",
        "inventory",
        "enter",
        "back"
    };
    return names[static_cast<int>(action)];
}

Action InputManager::GetActionFromName(const char* name) {
    static std::unordered_map<std::string, Action> actionMap = {
        {"move_left", Action::MOVE_LEFT},
        {"move_right", Action::MOVE_RIGHT},
        {"move_up", Action::MOVE_UP},
        {"move_down", Action::MOVE_DOWN},
        {"interact", Action::INTERACT},
        {"inventory", Action::INVENTORY},
        {"enter", Action::ENTER},
        {"back", Action::ESCAPE}
    };
    
    auto it = actionMap.find(name);
    return it != actionMap.end() ? it->second : Action::COUNT;
}
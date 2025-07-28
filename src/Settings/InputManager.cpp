#include "InputManager.h"
#include "Utilities/Input/KeysTranslate.h"

InputManager& InputManager::GetInstance() 
{
    static InputManager instance;
    return instance;
}

InputManager::InputManager() 
{
    LoadDefaultBinds();
}

void InputManager::LoadDefaultBinds()
{
    keyBindings[Action::MOVE_LEFT] = KEY_A;
    keyBindings[Action::MOVE_UP] = KEY_W;
    keyBindings[Action::MOVE_DOWN] = KEY_S;
    keyBindings[Action::MOVE_RIGHT] = KEY_D;
    keyBindings[Action::INVENTORY] = KEY_TAB;
    keyBindings[Action::ENTER] = KEY_ENTER;
    keyBindings[Action::ESCAPE] = KEY_ESCAPE;
}

void InputManager::SetKeyBind(Action action, int key) 
{
    keyBindings[action] = key;
}

int InputManager::GetKeyBind(Action action) const 
{
    auto it = keyBindings.find(action);
    return it != keyBindings.end() ? it->second : 0;
}

bool InputManager::IsActionDown(Action action) const 
{
    return IsKeyDown(GetKeyBind(action));
}

bool InputManager::IsActionPressed(Action action) const 
{
    return IsKeyPressed(GetKeyBind(action));
}

const char* InputManager::GetNameFromAction(Action action) 
{
    static const char* names[] = {
        "move_left",
        "move_up",
        "move_down",
        "move_right",
        "inventory",
        "enter",
        "back"
    };
    return names[static_cast<int>(action)];
}

Action InputManager::GetActionFromName(const char* name) 
{
    static std::unordered_map<std::string, Action> actionMap = {
        {"move_left", Action::MOVE_LEFT},
        {"move_up", Action::MOVE_UP},
        {"move_down", Action::MOVE_DOWN},
        {"move_right", Action::MOVE_RIGHT},
        {"inventory", Action::INVENTORY},
        {"enter", Action::ENTER},
        {"back", Action::ESCAPE}
    };
    
    auto it = actionMap.find(name);
    return it != actionMap.end() ? it->second : Action::ActionSize;
}
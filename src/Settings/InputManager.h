#pragma once
#include <raylib.h>
#include <unordered_map>

enum class Action {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    DASH,
    INTERACT,
    OPEN_INVENTORY,
    COUNT
};

class InputManager {
public:
    static InputManager& GetInstance();
    
    void SetKeyBinding(Action action, int key);
    int GetKeyBinding(Action action) const;
    
    bool IsActionPressed(Action action) const;
    bool IsActionJustPressed(Action action) const;
    
    static const char* GetActionName(Action action);
    static Action GetActionFromName(const char* name);

private:
    InputManager();
    void LoadDefaultBindings();
    
    std::unordered_map<Action, int> keyBindings;
};
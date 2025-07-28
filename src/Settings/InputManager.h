#pragma once
#include <raylib.h>
#include <unordered_map>

enum class Action {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    INTERACT,
    INVENTORY,
    ENTER,
    ESCAPE,
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
    void LoadDefaultBindings();

private:
    InputManager();
    
    std::unordered_map<Action, int> keyBindings;
};
#pragma once
#include <raylib.h>
#include <unordered_map>

/*

how add new action
1. add it into enum Action
2. LoadDefaultBinds()  add it there
3. GetNameFromAction() add it there
4. GetActionFromName() add it there
5. data/Settings/Settings.cfg add it there example:

(jump=E) 
first part (jump) must be equal to GetNameFromAction and GetActionFromName
second part (E) must be capitalized or have special key in Utilities/Input/KeysTranslate.h functions GetNameFromKey and GetKeyFromName

*/

enum class Action {
    MOVE_LEFT,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_RIGHT,
    INVENTORY,
    ENTER,
    ESCAPE,
    ActionSize // keep it last for enum size
};

class InputManager { // get key binds from settings config
public:
    static InputManager& GetInstance();
    void LoadDefaultBinds();
    
    // to get input
    bool IsActionDown(Action action) const;
    bool IsActionPressed(Action action) const;
    
    // for settings
    void SetKeyBind(Action action, int key);
    int GetKeyBind(Action action) const;

    static const char* GetNameFromAction(Action action);
    static Action GetActionFromName(const char* name);

private:
    InputManager();
    std::unordered_map<Action, int> keyBindings;
};
#include <string>
#include <raylib.h>
#include "Utilities/Logger/Logger.h"

inline std::string GetNameFromKey(int key) 
{
    // A-Z
    if (key >= KEY_A && key <= KEY_Z) { return std::string(1, 'A' + (key - KEY_A)); }

    // 0-9
    if (key >= KEY_ZERO && key <= KEY_NINE) { return std::string(1, '0' + (key - KEY_ZERO)); }

    // F1–F12
    if (key >= KEY_F1 && key <= KEY_F12) { return "F" + std::to_string(key - KEY_F1 + 1); }

    // special
    switch (key) 
    {
        case KEY_SPACE: return "SPACE";
        case KEY_ENTER: return "ENTER";
        case KEY_TAB: return "TAB";
        case KEY_ESCAPE: return "ESCAPE";
        case KEY_BACKSPACE: return "BACKSPACE";
        case KEY_DELETE: return "DELETE";

        case KEY_LEFT: return "LEFT";
        case KEY_RIGHT: return "RIGHT";
        case KEY_UP: return "UP";
        case KEY_DOWN: return "DOWN";

        case KEY_LEFT_SHIFT: return "LEFT_SHIFT";
        case KEY_RIGHT_SHIFT: return "RIGHT_SHIFT";
        case KEY_LEFT_CONTROL: return "LEFT_CONTROL";
        case KEY_RIGHT_CONTROL: return "RIGHT_CONTROL";
        case KEY_LEFT_ALT: return "LEFT_ALT";
        case KEY_RIGHT_ALT: return "RIGHT_ALT";
    }

    return "UNKNOWN";
}

inline int GetKeyFromName(const std::string& name) 
{
    if (name.empty()) { mycerr << "empty key"; }

    std::string upper;
    for (char c : name) { upper += std::toupper(c);}

    // A-Z
    if (upper.size() == 1 && upper[0] >= 'A' && upper[0] <= 'Z') { return KEY_A + (upper[0] - 'A'); }

    // 0–9
    if (upper.size() == 1 && upper[0] >= '0' && upper[0] <= '9') { return KEY_ZERO + (upper[0] - '0'); }

    // F1–F12
    if (upper[0] == 'F' && upper.size() <= 3) 
    {
        int num = std::stoi(upper.substr(1));
        if (num >= 1 && num <= 12) { return KEY_F1 + (num - 1); }
    }

    // special
    if (upper == "SPACE")     return KEY_SPACE;
    if (upper == "ENTER")     return KEY_ENTER;
    if (upper == "TAB")       return KEY_TAB;
    if (upper == "ESCAPE")    return KEY_ESCAPE;
    if (upper == "BACKSPACE") return KEY_BACKSPACE;
    if (upper == "DELETE")    return KEY_DELETE;

    if (upper == "LEFT")      return KEY_LEFT;
    if (upper == "RIGHT")     return KEY_RIGHT;
    if (upper == "UP")        return KEY_UP;
    if (upper == "DOWN")      return KEY_DOWN;

    if (upper == "LEFT_SHIFT")    return KEY_LEFT_SHIFT;
    if (upper == "RIGHT_SHIFT")   return KEY_RIGHT_SHIFT;
    if (upper == "LEFT_CONTROL")  return KEY_LEFT_CONTROL;
    if (upper == "RIGHT_CONTROL") return KEY_RIGHT_CONTROL;
    if (upper == "LEFT_ALT")      return KEY_LEFT_ALT;
    if (upper == "RIGHT_ALT")     return KEY_RIGHT_ALT;

    return 0; // KEY_NULL
}

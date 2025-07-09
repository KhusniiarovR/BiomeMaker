#ifndef INPUTANY_H
#define INPUTANY_H

#include "raylib.h"

inline bool inputAny() {
    
    if (GetKeyPressed() != 0) return true;

    for (int i = 0; i < 3; i++) {
        if (IsMouseButtonPressed(i)) return true;
    }

    if (GetMouseWheelMove() != 0.0f) return true;

    return false;
}

#endif // INPUTANY_H
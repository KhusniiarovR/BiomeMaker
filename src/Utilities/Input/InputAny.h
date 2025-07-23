#pragma once
#include "raylib.h"

inline bool inputAny() // checks for every keyboard or mouse interactions
{
    if (GetKeyPressed() != 0) { return true; } // keyboard

    for (int i = 0; i < 3; i++) { if (IsMouseButtonPressed(i)) return true; } // mouse buttons left middle right
    if (GetMouseWheelMove() != 0.0f) return true; // mouse wheel move
    return false;
}
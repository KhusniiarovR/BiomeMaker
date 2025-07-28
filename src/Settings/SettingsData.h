#pragma once
#include "raylib.h"

struct SettingsData {
    int windowWidth = GetMonitorWidth(0);
    int windowHeight = GetMonitorHeight(0);
    int maxFPS = 60;
};

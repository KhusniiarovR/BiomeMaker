#ifndef TEXT_H
#define TEXT_H

#include "raylib.h"
#include <string>
#include <iostream>

struct Text {
    std::string str;
    Vector2 position = {0.5f, 0.5f};
    float size = 32.0f;
    Color color = WHITE;
    float spacing = 4.0f;
    Font font = GetFontDefault();
};

// todo default font is missing, main init reason, add text handler class

inline void drawText(const Text &text) {
    if (text.font.texture.id == 0) {
        std::cout << "Warning: font texture is invalid!\n";
    }
    Vector2 dimensions = MeasureTextEx(text.font, text.str.c_str(), text.size, text.spacing);
    Vector2 pos = {
        (screenSizeX * text.position.x) - (0.5f * dimensions.x),
        (screenSizeY * text.position.y) - (0.5f * dimensions.y)
    };
    DrawTextEx(text.font, text.str.c_str(), pos, dimensions.y, text.spacing, text.color);
}

#endif //TEXT_H

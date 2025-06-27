#ifndef BUTTON_H
#define BUTTON_H

#include <raylib.h>
#include "Core/Render/Renderer.h"
#include <string>
#include <functional>

class Button {
public:
    Button(Vector2 pos, Vector2 size, std::string text, Color textColor = WHITE, float rounding = 0.0f, Color buttonColor = BLACK);
    void setTexture(Texture2D& tex);
    void setOnClick(std::function<void()> handler);

    void update(Vector2 mouseVirtual);
    void render(Renderer& renderer) const;

private:
    Vector2 position;
    Vector2 size;
    std::string text;
    Color textColor;
    Color buttonColor;
    float rounding = 0.0f;
    Texture2D texture = {0};
    bool hasTexture = false;
    bool isHovered = false;

    std::function<void()> onClickHandler;
};

#endif //BUTTON_H

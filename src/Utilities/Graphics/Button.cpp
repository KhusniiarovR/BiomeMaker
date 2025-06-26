#include "Button.h"
#include <utility>
#include <Constants/GraphicsConst.h>

Button::Button(Vector2 pos, Vector2 size, std::string text, Color textColor, float rounding)
    : position({pos.x * screenSizeX, pos.y * screenSizeY}),
    size({size.x * screenSizeX, size.y * screenSizeY}),
    text(std::move(text)),
    textColor(textColor),
    rounding(rounding) {}

Button::Button(float posX, float posY, float sizeX, float sizeY, std::string text, Color textColor, float rounding)
    : position({posX * screenSizeX, posY * screenSizeY}),
    size({sizeX * screenSizeX, sizeY * screenSizeY}),
    text(std::move(text)),
    textColor(textColor),
    rounding(rounding) {}

void Button::setTexture(Texture2D& tex) {
    texture = tex;
    hasTexture = true;
}

void Button::setOnClick(std::function<void()> handler) {
    onClickHandler = std::move(handler);
}

void Button::update() {
    Rectangle rect = { position.x, position.y, size.x, size.y };
    if (CheckCollisionPointRec(GetMousePosition(), rect)) {
        isHovered = true;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (onClickHandler) onClickHandler();
        }
    }
    else isHovered = false;
}

void Button::render(Renderer& renderer) const {
    Rectangle rect = { position.x, position.y, size.x, size.y };

    if (hasTexture) {
        Rectangle source;
        source.width = (float)texture.width / 2;
        source.height = (float)texture.height;
        source.x = isHovered ? source.width : 0;
        source.y = 0;
        DrawTexturePro(texture, source, rect, {0, 0}, 0.0f, WHITE);
    }
    else {
        Color color = isHovered ? WHITE : LIGHTGRAY;
        DrawRectangleRounded(rect, rounding, 10, color);
    }

    renderer.drawText(text, {(position.x + (size.x / 2)) / screenSizeX, (position.y + (size.y / 2)) / screenSizeY}, size.x / 5, textColor);
}


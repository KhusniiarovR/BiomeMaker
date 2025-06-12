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

void Button::setTexture(Texture2D tex) {
    texture = tex;
    hasTexture = true;
}

void Button::setOnClick(std::function<void()> handler) {
    onClickHandler = std::move(handler);
}

void Button::render(Renderer& renderer) const {
    Rectangle rect = { position.x, position.y, size.x, size.y };

    DrawRectangleRounded(rect, rounding, 10, LIGHTGRAY);

    if (hasTexture) {
        DrawTexturePro(texture,{0, 0, (float)texture.width, (float)texture.height },
                       rect, {0, 0}, 0.0f, WHITE);
    }
    DrawRectangleRoundedLines(rect, rounding, 12, DARKGRAY);

    renderer.drawText(text, {(position.x + (size.x / 2)) / screenSizeX, (position.y + (size.y / 2)) / screenSizeY}, size.x / 5, textColor);
}

void Button::update() {
    Rectangle rect = { position.x, position.y, size.x, size.y };
    if (CheckCollisionPointRec(GetMousePosition(), rect)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (onClickHandler) onClickHandler();
        }
    }
}

#include "Button.h"
#include <utility>
#include <Constants/GraphicsConst.h>

Button::Button(Vector2 pos, Vector2 size, std::string text, Color textColor, float rounding, Color buttonColor)
    : position({pos.x * virtualScreenSizeX, pos.y * virtualScreenSizeY}),
    size({size.x * virtualScreenSizeX, size.y * virtualScreenSizeY}),
    text(std::move(text)),
    textColor(textColor),
    rounding(rounding),
    buttonColor(buttonColor) {}

void Button::setTexture(Texture2D& tex) {
    texture = tex;
    hasTexture = true;
}

void Button::setOnClick(std::function<void()> handler) {
    onClickHandler = std::move(handler);
}

void Button::update(Vector2 mouseVirtual) {
    Rectangle rect = { position.x, position.y, size.x, size.y };
    if (CheckCollisionPointRec(mouseVirtual, rect)) {
        isHovered = true;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (onClickHandler) onClickHandler();
        }
    } 
    else {
        isHovered = false;
    }
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
        DrawRectangleRounded(rect, rounding, 10, isHovered ? DARKGRAY : buttonColor);
    }

    Vector2 center = { position.x + size.x / 2.0f, position.y + size.y / 2.0f };
    float fontSize = std::min(size.y * 0.5f, size.x / (isHovered ? 4.0f : 5.0f));

    renderer.drawText(text, center, fontSize,
        isHovered ? YELLOW : textColor,
        true, false);
}

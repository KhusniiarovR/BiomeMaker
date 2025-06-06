#include "Renderer.h"
#include "Constants/GraphicsConst.h"
#include <iostream>

Renderer::Renderer(AssetManager& assets) : assetManager(assets) {
    camera.offset = {0, 0};
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    camera.target = {0, 0};
}

Camera2D& Renderer::GetCamera() {
    return camera;
}

void Renderer::updateCamera(Vector2 playerPos) {
    camera.target = playerPos;
}

void Renderer::drawText(const std::string& text, Vector2 position,
                           float size, Color color,
                           const std::string& fontKey, float spacing) {

    const Font& font = assetManager.getFont(fontKey, size);

    if (font.texture.id == 0) {
            std::cout << "Renderer/drawText: font " << fontKey << " is invalid!\n";
    }

    Vector2 dimensions = MeasureTextEx(font, text.c_str(), size, spacing);
    Vector2 pos = {
        (screenSizeX * position.x) - (0.5f * dimensions.x),
        (screenSizeY * position.y) - (0.5f * dimensions.y)
    };

    DrawTextEx(font, text.c_str(), pos, dimensions.y, spacing, color);
}


void Renderer::drawPlayer(Vector2 playerPos) {
    DrawCircleV(playerPos, 50, RED);
}


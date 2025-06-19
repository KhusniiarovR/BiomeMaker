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

void Renderer::drawTextGradient(const std::string& text, Vector2 position,
                                float size, float speed,
                                Color colorA, Color colorB,
                                float bounceSpeed, float bounceHeight,
                                const std::string& fontKey, float spacing) {
    // todo not be that lazy and merge drawText functions + option to turn off centralization

    const Font& font = assetManager.getFont(fontKey, size);

    if (font.texture.id == 0) {
        std::cout << "Renderer/drawTextGradient: font " << fontKey << " is invalid!\n";
        return;
    }

    Vector2 totalDim = MeasureTextEx(font, text.c_str(), size, spacing);
    Vector2 basePos = {
        (screenSizeX * position.x) - (0.5f * totalDim.x),
        (screenSizeY * position.y) - (0.5f * totalDim.y)
    };

    Vector2 pos = basePos;
    float time = GetTime() * speed;

    for (int i = 0; i < (int)text.length(); ++i) {
        char ch[2] = { text[i], 0 };
        Vector2 charDim = MeasureTextEx(font, ch, size, spacing);

        float t = 0.5f + 0.5f * sinf(time + i * 0.3f);
        Color blended = {
            (unsigned char)(colorA.r + t * (colorB.r - colorA.r)),
            (unsigned char)(colorA.g + t * (colorB.g - colorA.g)),
            (unsigned char)(colorA.b + t * (colorB.b - colorA.b)),
            (unsigned char)(colorA.a + t * (colorB.a - colorA.a))
        };

        Vector2 charPos = pos;
        charPos.y += fabsf(sinf(time * bounceSpeed + i * 0.1f)) * bounceHeight;
        DrawTextEx(font, ch, charPos, size, spacing, blended);
        pos.x += charDim.x + spacing;
    }
}

void Renderer::drawPlayer(Vector2 playerPos) {
    DrawCircleV(playerPos, 50, RED);
}


Texture2D& Renderer::getTexture(const std::string& key) {
    return assetManager.getTexture(key);
}
#include "Renderer.h"
#include "Constants/GraphicsConst.h"
#include "Utilities/Logger/Logger.h"
#include "Constants/WorldConst.h"

Renderer::Renderer(AssetManager& assets, int width, int height, Vector2& mouseVirtual)
: assetManager(assets), virtualWidth(width), virtualHeight(height), mouseVirtual(mouseVirtual) {
    camera.offset = {0, 0};
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    camera.target = {0, 0};
}

Camera2D& Renderer::GetCamera() {
    return camera;
}

void Renderer::updateCamera(Vector2 position) {
    camera.target = position;
}

Vector2 Renderer::getMouseVirtual() {
    return mouseVirtual;
}

void Renderer::drawText(const std::string& text, Vector2 position,
                        float size, Color color,
                        bool isCentered, bool isNormalizedPos,
                        const std::string& fontKey, float spacing) {

    const Font& font = assetManager.getFont(fontKey, size);
    if (font.texture.id == 0) {
        mycerr << "font " << fontKey << " is invalid!";
        return;
    }

    if (isNormalizedPos) {
        position.x *= virtualWidth;
        position.y *= virtualHeight;
    }

    Vector2 dimensions = MeasureTextEx(font, text.c_str(), size, spacing);

    if (isCentered) {
        position.x -= 0.5f * dimensions.x;
        position.y -= 0.5f * dimensions.y;
    }

    DrawTextEx(font, text.c_str(), position, size, spacing, color);
}

void Renderer::drawTextGradient(const std::string& text, Vector2 position,
                                float size, float speed,
                                Color colorA, Color colorB,
                                bool isNormalizedPos, bool isCentered,
                                float bounceSpeed, float bounceHeight,
                                const std::string& fontKey, float spacing) {
    
    const Font& font = assetManager.getFont(fontKey, size);
    if (font.texture.id == 0) {
        mycerr << "font " << fontKey << " is invalid!";
        return;
    }

    if (isNormalizedPos) {
        position.x *= virtualWidth;
        position.y *= virtualHeight;
    }

    Vector2 totalDim = MeasureTextEx(font, text.c_str(), size, spacing);

    Vector2 basePos = position;
    if (isCentered) {
        basePos.x -= 0.5f * totalDim.x;
        basePos.y -= 0.5f * totalDim.y;
    }

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

void Renderer::drawBackground() {
    const int pixelheight = 30;
    Texture2D bg = getTexture("background", true);
    bgoffset += GetFrameTime() * 100;

    float scale = (float)virtualHeight / pixelheight;
    float sourceWidth = (float)virtualWidth / scale;

    Rectangle sourceRec = { bgoffset / scale, 0, sourceWidth, (float)bg.height };
    Rectangle destRec = { 0, 0, (float)virtualWidth, (float)virtualHeight };

    DrawTexturePro(bg, sourceRec, destRec, {0, 0}, 0.0f, WHITE);
} 

void Renderer::drawPlayer(const Player& player) {
    Texture2D pl = getTexture(player.lastDirection, true);

    Rectangle source = { 0, 0, (float)pl.width, (float)pl.height };
    Rectangle dest = { player.position.x, player.position.y, (float)pl.width, (float)pl.height };
    Vector2 origin = { pl.width / 2.0f, pl.height / 2.0f };
    float rotation = player.rotation;
    //я случайно убрал желтый круг, сорян
DrawTexturePro(pl, source, dest, origin, rotation, WHITE);
}

void Renderer::drawEnemy(const Enemy& enemy) {
    Texture2D en = getTexture("enemy"+enemy.currentDir, true);
    Rectangle source = { 0, 0, (float)en.width, (float)en.height };
    Rectangle dest = { enemy.position.x, enemy.position.y, (float)en.width, (float)en.height };
    Vector2 origin = { en.width / 2.0f, en.height / 2.0f };
    float rotation = enemy.rotation;

DrawTexturePro(en, source, dest, origin, rotation, WHITE);
}

Texture2D& Renderer::getTexture(const std::string& key, bool shouldBeWrapped) {
    return assetManager.getTexture(key, shouldBeWrapped);
}
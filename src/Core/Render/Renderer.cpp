#include "Renderer.h"
#include "Constants/GraphicsConst.h"
#include "Utilities/Logger/Logger.h"
#include "Constants/WorldConst.h"
#include "Constants/TilemapConst.h"
#include <cmath>

Renderer::Renderer(AssetManager& assets, int width, int height, Vector2& mouseVirtual)
: assetManager(assets), virtualWidth(width), virtualHeight(height), mouseVirtual(mouseVirtual) 
{
    camera.offset = {0, 0};
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    camera.target = {0, 0};
}

Camera2D& Renderer::GetCamera() 
{
    return camera;
}

void Renderer::updateCameraTarget(Vector2 position) 
{
    camera.target = position;
}

Vector2 Renderer::getMouseVirtual() 
{
    return mouseVirtual;
}

void Renderer::drawText(const std::string& text, Vector2 position,
                        float size, Color color,
                        bool isCentered, bool isNormalizedPos,
                        const std::string& fontKey, float spacing) 
{
    const Font& font = assetManager.getFont(fontKey, size);
    if (font.texture.id == 0) 
    {
        mycerr << "font " << fontKey << " is invalid!";
        return;
    }

    // if position in percents of screen or in pixels
    if (isNormalizedPos) 
    { 
        position.x *= virtualWidth;
        position.y *= virtualHeight;
    }
    
    Vector2 dimensions = MeasureTextEx(font, text.c_str(), size, spacing);
    
    // draw from position or make center of the text equal to position
    if (isCentered) 
    {
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
                                const std::string& fontKey, float spacing)
{
    const Font& font = assetManager.getFont(fontKey, size);
    if (font.texture.id == 0) 
    {
        mycerr << "font " << fontKey << " is invalid!";
        return;
    }

    // if position in percents of screen or in pixels
    if (isNormalizedPos)
    {
        position.x *= virtualWidth;
        position.y *= virtualHeight;
    }

    Vector2 totalDim = MeasureTextEx(font, text.c_str(), size, spacing);

    Vector2 basePos = position;
    // draw from position or make center of the text equal to position
    if (isCentered) 
    {
        basePos.x -= 0.5f * totalDim.x;
        basePos.y -= 0.5f * totalDim.y;
    }

    Vector2 pos = basePos;
    float time = GetTime() * speed;

    // jumping letter
    for (int i = 0; i < (int)text.length(); ++i) 
    {
        char ch[2] = { text[i], 0 };
        Vector2 charDim = MeasureTextEx(font, ch, size, spacing);

        float t = 0.5f + 0.5f * sinf(time + i * 0.3f);
        Color blended = { (unsigned char)(colorA.r + t * (colorB.r - colorA.r)), (unsigned char)(colorA.g + t * (colorB.g - colorA.g)),
                          (unsigned char)(colorA.b + t * (colorB.b - colorA.b)), (unsigned char)(colorA.a + t * (colorB.a - colorA.a)) };

        Vector2 charPos = pos;
        charPos.y += fabsf(sinf(time * bounceSpeed + i * 0.1f)) * bounceHeight;
        DrawTextEx(font, ch, charPos, size, spacing, blended);
        pos.x += charDim.x + spacing;
    }
}

void Renderer::drawBackground() 
{
    const int pixelheight = 30;
    Texture2D& bg = getTexture("background", true);
    bgoffset += GetFrameTime() * 20;

    float scale = (float)virtualHeight / pixelheight;
    float sourceWidth = (float)virtualWidth / scale;

    Rectangle sourceRec = { bgoffset / scale, 0, sourceWidth, (float)bg.height };
    Rectangle destRec = { 0, 0, (float)virtualWidth, (float)virtualHeight };

    DrawTexturePro(bg, sourceRec, destRec, {0, 0}, 0.0f, WHITE);
}

Texture2D& Renderer::getTexture(const std::string& key, bool shouldBeWrapped) 
{
    return assetManager.getTexture(key, shouldBeWrapped);
}

Font& Renderer::getFont(const std::string& key, const int size) {
    return assetManager.getFont(key, size);
}

Sound& Renderer::getSound(const std::string& key) {
    return assetManager.getSound(key);
}
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

void Renderer::drawInventory(const Inventory* inventory) {
    if (!inventory) {
        mycerr << "inventory == nullptr";
        return;
    }

    const int padding = 5;
    const int columns = 10;
    const int slotSize = 35;
    const Vector2 position = {120, 5};
    Texture2D& itemTilemap = getTexture("itemTilemap");

    for (int i = 0; i < Inventory::SLOT_COUNT; ++i) {
        int col = i % columns;
        int row = i / columns;

        float x = position.x + col * (slotSize + padding);
        float y = position.y + row * (slotSize + padding);
        Rectangle slotRect = { x, y, (float)slotSize, (float)slotSize };

        DrawRectangleRec(slotRect, DARKGRAY);
        DrawRectangleLinesEx(slotRect, 2, LIGHTGRAY);

        const ItemStack& stack = inventory->getSlot(i);
        if (!stack.isEmpty()) {
            const Item& item = stack.getItem();

            float iconSize = slotSize * 0.8f;
            float iconX = x + (slotSize - iconSize) * 0.5f;
            float iconY = y + (slotSize - iconSize) * 0.5f;

            Rectangle src = item.getIconSourceRect();
            Rectangle dst = { iconX, iconY, iconSize, iconSize };

            DrawTexturePro(itemTilemap, src, dst, {0, 0}, 0.0f, WHITE);

            if (stack.count > 1) {
                std::string countText = std::to_string(stack.count);
                int fontSize = 12;
                Vector2 textSize = MeasureTextEx(GetFontDefault(), countText.c_str(), (float)fontSize, 1);
                DrawText(countText.c_str(), x + slotSize - textSize.x - 2, y + slotSize - textSize.y, fontSize, WHITE);
            }
        }
    }
}



Texture2D& Renderer::getTexture(const std::string& key, bool shouldBeWrapped) {
    return assetManager.getTexture(key, shouldBeWrapped);
}
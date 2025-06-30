#include "TextBox.h"

TextBox::TextBox(float x, float y, float width, float height, Color BGcolor, Color lineColor, Color activeLineColor, Color textColor, int textSize) :
box{ virtualScreenSizeX*x, virtualScreenSizeY*y, virtualScreenSizeX*width, virtualScreenSizeY*height }, 
BGColor{BGcolor}, lineColor{lineColor},
textColor{textColor}, textSize(textSize) {}

void TextBox::update(float dt, Vector2 mouseVirtual)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        active = CheckCollisionPointRec(mouseVirtual, box);
    }
    if (active)
    {
        int key = GetCharPressed();
        while (key > 0)
        {
            if ((key >= 32) && (key <= 125))
            {
                text += (char)key;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !text.empty())
        {
            text.pop_back();
            backspaceTimer = 0.6f; // задержка впервый раз
        }
        else if (IsKeyDown(KEY_BACKSPACE) && !text.empty()) 
        {
            backspaceTimer -= dt;
            if (backspaceTimer <= 0.0f) 
            {
                text.pop_back();
                backspaceTimer = 0.05f; // задержка в последующие
            }
        }
        else
        {
            backspaceTimer = 0.0f; 
        }
    }
    if(CheckCollisionPointRec(mouseVirtual, box))
    {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT); 
    }
}
void TextBox::draw(Renderer& renderer) const
{
    DrawRectangleRec(box,BGColor);
    if(active)
        DrawRectangleLines((int)box.x, (int)box.y, (int)box.width, (int)box.height, activeLineColor);
    else
        DrawRectangleLines((int)box.x, (int)box.y, (int)box.width, (int)box.height, lineColor);
    renderer.drawText(text.c_str(), {box.x, box.y}, textSize, textColor, false, false, "inter", 0.3f);
}
string TextBox::returnText()
{
    return text;
}

void TextBox::eraseText() 
{
    text.clear();
}
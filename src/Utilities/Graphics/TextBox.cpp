#include "TextBox.h"
void TextBox::update(float dt)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        active = CheckCollisionPointRec(GetMousePosition(), box);
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
    if(CheckCollisionPointRec(GetMousePosition(), box))
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
    DrawRectangleLines((int)box.x, (int)box.y, (int)box.width, (int)box.height, lineColor);
    renderer.drawText(text.c_str(), {box.x, box.y}, textSize, textColor, false, false, "inter", 0.3f);
}
string TextBox::returnText()
{
    return text;
}
#include "TextBox.h"
#include <utility>
#include <Constants/GraphicsConst.h>
void TextBox::update()
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mousePos = GetMousePosition();
        active = CheckCollisionPointRec(mousePos, box);
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
    DrawRectangleRec(box,LIGHTGRAY);
    DrawRectangleLines((int)box.x, (int)box.y, (int)box.width, (int)box.height, WHITE);
    DrawText(text.c_str(), box.x + 5, box.y + 8, 20, BLACK);
}

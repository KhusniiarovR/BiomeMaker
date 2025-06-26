
#pragma once 
#include <raylib.h>
#include <string>
#include <Core/Render/Renderer.h>
#include <Constants/GraphicsConst.h>
using namespace std;
class TextBox
{
    private:
        Rectangle box;
        string text;
        bool active;    
        Color BGColor;
        Color lineColor;
        Color textColor;
        float backspaceTimer = 0.0f;
        int textSize;
    
    public:
        TextBox(float x, float y, float width, float height,Color BGcolor, Color lineColor, Color textColor, int textSize) : textSize(textSize)
        {
            box = { screenSizeX*x, screenSizeY*y, screenSizeX*width, screenSizeY*height };
            text = "";
            active = false;
            BGColor = BGcolor;
            lineColor = lineColor;
            textColor = textColor;
        }
        void update(float dt);
        void draw(Renderer& renderer) const;
        string returnText();
};
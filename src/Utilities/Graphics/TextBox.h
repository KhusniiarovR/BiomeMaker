
#pragma once 
#include <raylib.h>
#include <string>
#include <Core/Render/Renderer.h>
using namespace std;
class TextBox
{
    private:
        Rectangle box;
        string text;
        bool active;    
        Color BGColor;
        Color lineColor;
    
    public:
        TextBox(float x, float y, float width, float height,Color BGcolor, Color lineColor)
        {
            box = { x, y, width, height };
            text = "";
            active = false;
            BGColor = BGcolor;
            lineColor = lineColor;
        }
        void update();
        void draw(Renderer& renderer) const;
};

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
    
    public:
        TextBox(float x, float y, float width, float height)
        {
            box = { x, y, width, height };
            text = "";
            active = false;
        }
        void update();
        void draw(Renderer& renderer) const;
};
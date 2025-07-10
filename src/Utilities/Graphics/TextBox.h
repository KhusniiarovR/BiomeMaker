
#pragma once 
#include <raylib.h>
#include <string>
#include <Core/Render/Renderer.h>
#include <Constants/GraphicsConst.h>
class TextBox
{
    private:
        Rectangle box;
        std::string text = "";
        bool active = false;    
        Color BGColor;
        Color lineColor;
        Color activeLineColor;
        Color textColor;
        float backspaceTimer = 0.0f;
        int textSize;

    public:
        TextBox(float x, float y, float width, float height,Color BGcolor, Color lineColor, Color activeLineColor, Color textColor, int textSize);
        void update(float dt, Vector2 mouseVirtual);
        void draw(Renderer& renderer) const;
        std::string returnText();
        void eraseText();
};
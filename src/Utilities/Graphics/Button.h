#pragma once
#include <raylib.h>
#include "Core/Render/Renderer.h"
#include <string>
#include <functional>

class Button {
public:
    Button(Vector2 pos, Vector2 size, std::string text, Color textColor = WHITE, float rounding = 0.0f, Color buttonColor = BLACK);
    void update(Vector2 mouseVirtual);
    void render(Renderer& renderer) const;

    void setTexture(Texture2D& tex);
    void setOnClick(std::function<void()> handler);

private:
    // position and size
    Vector2 position;
    Vector2 size;
    
    // text
    std::string text;
    Color textColor;

    // button beauty
    Texture2D texture = {0}; 
    float rounding = 0.0f; // to circle corners of a button
    Color buttonColor; // color if there's no texture

    bool hasTexture = false;
    bool isHovered = false;

    std::function<void()> onClickHandler; // button functionality
};

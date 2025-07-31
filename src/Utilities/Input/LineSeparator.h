#pragma once
#include <vector>
#include <string>
#include "raylib.h"

// divide text into several parts based on container width
inline std::vector<std::string> separateText(const std::string& text, Font& font, float fontSize, float maxWidth) 
{
    std::vector<std::string> lines;
    std::string currentLine;
    std::string currentWord;
    float spaceWidth = MeasureTextEx(font, " ", fontSize, 1).x;

    for (char c : text) 
    {
        if (c == ' ') 
        {
            float lineWidth = MeasureTextEx(font, currentLine.c_str(), fontSize, 1).x;
            float wordWidth = MeasureTextEx(font, currentWord.c_str(), fontSize, 1).x;
            float totalWidth = lineWidth + (currentLine.empty() ? 0.0f : spaceWidth) + wordWidth;

            if (totalWidth > maxWidth) 
            {
                if (!currentLine.empty()) { lines.push_back(currentLine); }
                currentLine = currentWord;
            }
            else
            {
                if (!currentLine.empty()) { currentLine += " "; }
                currentLine += currentWord;
            }

            currentWord.clear();
        } 
        else if (c == '\n') 
        {
            currentLine += currentWord;
            lines.push_back(currentLine);
            currentLine.clear();
            currentWord.clear();
        } 
        else { currentWord += c; }
    }

    if (!currentWord.empty()) 
    {
        float lineWidth = MeasureTextEx(font, currentLine.c_str(), fontSize, 1).x;
        float wordWidth = MeasureTextEx(font, currentWord.c_str(), fontSize, 1).x;
        float totalWidth = lineWidth + (currentLine.empty() ? 0.0f : spaceWidth) + wordWidth;

        if (totalWidth > maxWidth && !currentLine.empty()) 
        {
            lines.push_back(currentLine);
            currentLine = currentWord;
        } 
        else 
        {
            if (!currentLine.empty()) { currentLine += " "; }
            currentLine += currentWord;
        }
    }

    if (!currentLine.empty()) { lines.push_back(currentLine); }

    return lines;
}


#pragma once
#include <string>

struct AnimationBase {
    std::string name;
    int startFrame; // index in tilemap
    int frameCount; // how much frames animation has
    float frameTime; // pause between frames
    bool loop;
};

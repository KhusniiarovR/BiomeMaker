#pragma once
#include <string>

struct AnimationBase {
    std::string name;
    int startFrame;
    int frameCount;
    float frameTime;
    bool loop;
};

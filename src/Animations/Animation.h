#pragma once
#include "AnimationBase.h"
#include <unordered_map>
#include "raylib.h"

class Animation {
public:
    Animation(int frameWidth, int frameHeight);

    void addAnimation(const AnimationBase& anim);
    void play(const std::string& name);
    void update(float deltaTime);
    void draw(Texture2D& texture, Vector2 position) const;
    void setFlip(bool f);
    bool isFlipped() const;

private:
    int frameWidth, frameHeight;
    bool flip = false;

    std::unordered_map<std::string, AnimationBase> animations;
    AnimationBase* currentAnimation = nullptr;

    float timer = 0.0f;
    int currentFrame = 0;
};

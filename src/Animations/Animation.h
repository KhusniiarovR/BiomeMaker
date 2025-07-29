#pragma once
#include "AnimationBase.h"
#include <unordered_map>
#include "raylib.h"

class Animation { // keeps all possible animations and play() picks one of them
private:
    int frameWidth, frameHeight;
    bool flip = false;

    std::unordered_map<std::string, AnimationBase> animations;
    AnimationBase* currentAnimation = nullptr;

    float timer = 0.0f;
    int currentFrame = 0;

public:
    Animation(int frameWidth, int frameHeight);
    void addAnimation(const AnimationBase& anim); 
    
    void play(const std::string& name);
    void update(float deltaTime);
    void render(Texture2D& texture, Vector2 position) const;

    void setFlip(bool f);
    bool isFlipped() const;
};

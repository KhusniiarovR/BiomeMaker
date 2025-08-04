#pragma once
#include "raylib.h"

class Knockback { // simulate knockback if took damage
private:
    Vector2 velocity = {0, 0};
    float timeLeft = 0.0f;
    const float maxTime = 0.15f;
    const float decay = 0.9f;

public:
    void apply(Vector2 source, Vector2 target, float strength = 200.0f);
    void update(float dt);
    bool isActive() const;
    Vector2 getCurrentOffset(float dt) const;
};

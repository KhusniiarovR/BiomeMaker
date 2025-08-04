#include "Knockback.h"
#include "raymath.h"

void Knockback::apply(Vector2 source, Vector2 target, float strength) 
{
    Vector2 dir = Vector2Subtract(target, source);
    dir = Vector2Normalize(dir);
    velocity = Vector2Scale(dir, strength);
    timeLeft = maxTime;
}

void Knockback::update(float dt) 
{
    if (timeLeft > 0.0f) 
    {
        timeLeft -= dt;
        velocity = Vector2Scale(velocity, decay);
    }
}

bool Knockback::isActive() const 
{
    return timeLeft > 0.0f;
}

Vector2 Knockback::getCurrentOffset(float dt) const 
{
    return isActive() ? Vector2Scale(velocity, dt) : Vector2{0, 0};
}

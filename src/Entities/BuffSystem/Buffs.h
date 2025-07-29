#pragma once

/*
to make new buff
1. add it to enum BuffType
2. in BuffSystem.cpp initEffects() make logic
3. if this is food then make food item items/ItemsAll/Food/ItemFood.cpp
*/

enum class BuffType { // list of buffs
    Heal,
    Speed
};

struct BuffEffect { // bufftype + level + time
    BuffType type;
    float value;
    float duration = 0.0f;

    BuffEffect(BuffType type, float value, float duration = 0.0f) : type(type), value(value), duration(duration) {}
};
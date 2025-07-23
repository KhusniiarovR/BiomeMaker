#pragma once

enum class BuffType { // list of buffs(debuffs)
    Heal,
    Speed
};

struct BuffEffect { // bufftype + level + time
    BuffType type;
    float value;
    float duration = 0.0f;

    BuffEffect(BuffType type, float value, float duration = 0.0f) : type(type), value(value), duration(duration) {}
};
#ifndef BUFFS_H
#define BUFFS_H

enum class BuffType {
    Heal,
    Speed
};


struct BuffEffect {
    BuffType type;
    float value;
    float duration = 0.0f;

    BuffEffect(BuffType type, float value, float duration = 0.0f)
        : type(type), value(value), duration(duration) {}
};



#endif // BUFFS_H
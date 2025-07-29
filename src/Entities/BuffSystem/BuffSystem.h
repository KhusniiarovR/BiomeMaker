#pragma once
#include <vector>
#include <functional>
#include <unordered_map>
#include "Buffs.h"

class Player; // forward declaration

struct ActiveBuff {
    BuffEffect effect;
    float remainingTime;

    ActiveBuff(const BuffEffect& effect);
};

class BuffSystem {
private:
    std::vector<ActiveBuff> buffs; // all buffs in this vector
    Player& player; // reference to entity
    // TODO not only for player buffs

    // to keep every buff in the map
    using EffectFunc = std::function<void(float)>;
    struct TimedEffect {
        EffectFunc apply;
        EffectFunc remove;
    };

    std::unordered_map<BuffType, TimedEffect> timedEffects; // buffs with duration
    std::unordered_map<BuffType, EffectFunc> instantEffects;// buffs without duration

public:
    explicit BuffSystem(Player& playerRef);
    void update(float dt);

    void addBuff(const BuffEffect& effect);
    
private:
    void applyEffectInstant(const BuffEffect& effect); // instant
    void applyEffectTimed(const BuffEffect& effect); // with duration
    void removeEffectTimed(const BuffEffect& effect); // remove from vector
    void initEffects(); // init buffs maps
};
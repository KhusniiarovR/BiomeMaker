#pragma once
#include <vector>
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

public:
    explicit BuffSystem(Player& playerRef);
    void update(float dt);

    void addBuff(const BuffEffect& effect);
    
private:
    void applyEffectImmediate(const BuffEffect& effect); // instant
    void applyEffectStart(const BuffEffect& effect); // with duration
    void removeEffect(const BuffEffect& effect); // remove from vector
};
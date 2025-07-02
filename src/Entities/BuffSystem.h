#ifndef BUFFSYSTEM_H
#define BUFFSYSTEM_H

#include <vector>
#include "Items/Buffs/Buffs.h"

class Player;

struct ActiveBuff {
    BuffEffect effect;
    float remainingTime;

    ActiveBuff(const BuffEffect& effect);
};

class ActiveBuffSystem {
private:
    std::vector<ActiveBuff> buffs;
    Player& player;

public:
    explicit ActiveBuffSystem(Player& playerRef);

    void addBuff(const BuffEffect& effect);
    void update(float dt);

private:
    void applyEffectImmediate(const BuffEffect& effect);
    void applyEffectStart(const BuffEffect& effect);
    void removeEffect(const BuffEffect& effect);
};

#endif // BUFFSYSTEM_H

#include "Entities/BuffSystem.h"
#include "Entities/Player.h"

ActiveBuff::ActiveBuff(const BuffEffect& effect)
    : effect(effect), remainingTime(effect.duration) {}


ActiveBuffSystem::ActiveBuffSystem(Player& playerRef) : player(playerRef) {}

void ActiveBuffSystem::addBuff(const BuffEffect& effect) {
    if (effect.duration <= 0.0f) {
        applyEffectImmediate(effect);
        return;
    }

    applyEffectStart(effect);
    buffs.emplace_back(effect);
}

void ActiveBuffSystem::update(float dt) {
    for (size_t i = 0; i < buffs.size(); ) {
        ActiveBuff& buff = buffs[i];
        buff.remainingTime -= dt;

        if (buff.remainingTime <= 0.0f) {
            removeEffect(buff.effect);
            buffs.erase(buffs.begin() + i);
        } else {
            ++i;
        }
    }
}

void ActiveBuffSystem::applyEffectImmediate(const BuffEffect& effect) {
    switch (effect.type) {
        case BuffType::Heal:
            player.heal(effect.value);
            break;
        default:
            break;
    }
}

void ActiveBuffSystem::applyEffectStart(const BuffEffect& effect) {
    switch (effect.type) {
        case BuffType::Speed:
            player.setSpeedMultiplier(player.getSpeedMultiplier() + effect.value);
            break;
        default:
            break;
    }
}

void ActiveBuffSystem::removeEffect(const BuffEffect& effect) {
    switch (effect.type) {
        case BuffType::Speed:
            player.setSpeedMultiplier(player.getSpeedMultiplier() - effect.value);
            break;
        default:
            break;
    }
}

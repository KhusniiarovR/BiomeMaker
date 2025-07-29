#include "BuffSystem.h"
#include "Entities/Player/Player.h"

ActiveBuff::ActiveBuff(const BuffEffect& effect) : effect(effect), remainingTime(effect.duration) {}

BuffSystem::BuffSystem(Player& playerRef) : player(playerRef) 
{
    initEffects();
}

void BuffSystem::addBuff(const BuffEffect& effect) 
{
    if (effect.duration <= 0.0f) 
    {
        applyEffectInstant(effect);
        return;
    }
    applyEffectTimed(effect);
    buffs.emplace_back(effect);
}

void BuffSystem::update(float dt) 
{

    for (size_t i = 0; i < buffs.size();) // update timer for every buff
    {
        ActiveBuff& buff = buffs[i];
        buff.remainingTime -= dt;

        if (buff.remainingTime <= 0.0f) 
        {
            removeEffectTimed(buff.effect);
            buffs.erase(buffs.begin() + i);
        } 
        else { ++i; }
    }
}

void BuffSystem::applyEffectInstant(const BuffEffect& effect) 
{
    auto it = instantEffects.find(effect.type);
    if (it != instantEffects.end()) { it->second(effect.value); }
}

void BuffSystem::applyEffectTimed(const BuffEffect& effect) 
{
    auto it = timedEffects.find(effect.type);
    if (it != timedEffects.end()) { it->second.apply(effect.value); }
}

void BuffSystem::removeEffectTimed(const BuffEffect& effect) 
{
    auto it = timedEffects.find(effect.type);
    if (it != timedEffects.end()) { it->second.remove(effect.value); }
}

void BuffSystem::initEffects() {
    // Instant buffs
    instantEffects[BuffType::Heal] = [this](float value) {
        player.heal(value);
    };

    // Timed buffs
    timedEffects[BuffType::Speed] = {
        [this](float value) { player.setSpeedMultiplier(player.getSpeedMultiplier() + value); },  // apply
        [this](float value) { player.setSpeedMultiplier(player.getSpeedMultiplier() - value); }   // remove
    };
}
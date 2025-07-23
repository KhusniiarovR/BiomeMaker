#include "BuffSystem.h"
#include "Entities/Player/Player.h"

ActiveBuff::ActiveBuff(const BuffEffect& effect) : effect(effect), remainingTime(effect.duration) {}

BuffSystem::BuffSystem(Player& playerRef) : player(playerRef) {}

void BuffSystem::addBuff(const BuffEffect& effect) 
{
    if (effect.duration <= 0.0f) 
    {
        applyEffectImmediate(effect);
        return;
    }
    applyEffectStart(effect);
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
            removeEffect(buff.effect);
            buffs.erase(buffs.begin() + i);
        } 
        else { ++i; }
    }
}

void BuffSystem::applyEffectImmediate(const BuffEffect& effect) 
{
    switch (effect.type) 
    {
        case BuffType::Heal:
            player.heal(effect.value);
        break;
        default:
        break;
    }
}

void BuffSystem::applyEffectStart(const BuffEffect& effect) 
{
    // TODO merge switch
    switch (effect.type) // everything here must be duplicated in removeEffect
    {
        case BuffType::Speed:
            player.setSpeedMultiplier(player.getSpeedMultiplier() + effect.value);
        break;
        default:
        break;
    }
}

void BuffSystem::removeEffect(const BuffEffect& effect) 
{
    switch (effect.type) // everything here must be duplicated in applyEffectStart
    {
        case BuffType::Speed:
            player.setSpeedMultiplier(player.getSpeedMultiplier() - effect.value);
        break;
        default:
        break;
    }
}

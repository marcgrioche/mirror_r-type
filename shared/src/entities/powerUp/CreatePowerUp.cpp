/*
** CreatePowerUp.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/entities/powerUp
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 1:54:34 PM 2025 jojo
** Last update Wed Oct 7 1:55:06 PM 2025 jojo
*/

#include "CreatePowerUp.hpp"
#include "components/Tags.hpp"

namespace factories {
Entity createPowerUp(Registry& registry,
    const Position& position,
    const Velocity& velocity,
    const Hitbox& hitbox,
    const Lifetime& lifetime,
    PowerUpType type,
    float effectDuration)
{
    Entity powerUp = registry.create_entity();
    registry.emplace<Position>(powerUp, position);
    registry.emplace<Velocity>(powerUp, velocity);
    registry.emplace<Hitbox>(powerUp, hitbox);
    registry.emplace<Lifetime>(powerUp, lifetime);
    registry.emplace<PowerUpTag>(powerUp);

    PowerUp powerUpComponent;
    powerUpComponent.type = type;
    powerUpComponent.effect_duration = effectDuration;
    registry.emplace<PowerUp>(powerUp, powerUpComponent);

    return powerUp;
}
}

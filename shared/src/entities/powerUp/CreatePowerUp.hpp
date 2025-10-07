/*
** CreatePowerUp.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/entities/powerUp
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 1:51:11 PM 2025 jojo
** Last update Wed Oct 7 1:52:12 PM 2025 jojo
*/

#pragma once
#include "Parent.hpp"
#include "Registry.hpp"
#include "components/AllComponents.hpp"

namespace factories {
Entity createPowerUp(Registry& registry,
    const Position& position,
    const Velocity& velocity,
    const Hitbox& hitbox,
    const Lifetime& lifetime,
    PowerUpType type,
    float effectDuration);
}

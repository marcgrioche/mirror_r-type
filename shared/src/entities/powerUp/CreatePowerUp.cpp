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

namespace factories {
Entity CreatePowerUp(Registry& registry,
    const Position& position,
    const Velocity& velocity,
    const Hitbox& hitbox,
    const Lifetime& lifetime)
{
    Entity projectile = registry.create_entity();
    registry.emplace<Position>(projectile, position);
    registry.emplace<Velocity>(projectile, velocity);
    registry.emplace<Hitbox>(projectile, hitbox);
    registry.emplace<Lifetime>(projectile, lifetime);
    registry.emplace<PowerUpTag>(projectile);
    return projectile;
}
}

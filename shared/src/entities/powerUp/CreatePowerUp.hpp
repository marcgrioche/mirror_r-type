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
#include "components/Hitbox.hpp"
#include "components/Lifetime.hpp"
#include "components/Position.hpp"
#include "components/PowerUp.hpp"
#include "components/Velocity.hpp"

namespace factories {
/**
 * @brief Creates a power-up entity with specified properties
 * @param registry The ECS registry to create the entity in
 * @param position The initial position of the power-up
 * @param velocity The velocity component for movement
 * @param hitbox The hitbox component for collision detection
 * @param lifetime The lifetime component for automatic removal
 * @param type The type of power-up effect
 * @param effectDuration The duration of the power-up effect when collected
 * @return The created power-up entity
 */
Entity createPowerUp(Registry& registry,
    const Position& position,
    const Velocity& velocity,
    const Hitbox& hitbox,
    const Lifetime& lifetime,
    PowerUpType type,
    float effectDuration);
}

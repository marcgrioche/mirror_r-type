/*
** CreateProjectile.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/projectil
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Sep 23 1:12:31 PM 2025 jojo
** Last update Wed Sep 23 1:12:32 PM 2025 jojo
*/

#pragma once
#include "Parent.hpp"
#include "Registry.hpp"
#include "components/Damage.hpp"
#include "components/Hitbox.hpp"
#include "components/Lifetime.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"

namespace factories {
/**
 * @brief Creates a projectile entity with specified properties
 * @param registry The ECS registry to create the entity in
 * @param position The initial position of the projectile
 * @param velocity The velocity component for movement
 * @param damage The damage component for collision effects
 * @param hitbox The hitbox component for collision detection
 * @param parent The parent entity that spawned this projectile
 * @param lifetime The lifetime component for automatic removal
 * @return The created projectile entity
 */
Entity createProjectile(Registry& registry,
    const Position& position,
    const Velocity& velocity,
    const Damage& damage,
    const Hitbox& hitbox,
    const Parent& parent,
    const Lifetime& lifetime);

/**
 * @brief Creates a projectile template entity without position/lifetime (used as blueprint)
 * @param registry The ECS registry to create the entity in
 * @param velocity The velocity component for movement
 * @param damage The damage component for collision effects
 * @param hitbox The hitbox component for collision detection
 * @return The created projectile template entity
 */
Entity createProjectileTemplate(
    Registry& registry,
    const Velocity& velocity,
    const Damage& damage,
    const Hitbox& hitbox);
}

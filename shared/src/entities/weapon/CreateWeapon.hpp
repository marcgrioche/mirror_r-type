/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** CreateWeapon
*/

#pragma once
#include "Entity.hpp"
#include "Parent.hpp"
#include "Registry.hpp"
#include <optional>

// always a parent so they can be turrets
namespace factories {
/**
 * @brief Creates a weapon entity with default properties
 * @param registry The ECS registry to create the entity in
 * @return The created weapon entity
 */
Entity createWeapon(Registry& registry);

/**
 * @brief Creates a weapon entity with specified parent and projectile template
 * @param registry The ECS registry to create the entity in
 * @param parent The parent entity that owns this weapon
 * @param projectile The projectile template entity to use for spawning projectiles
 * @return The created weapon entity
 */
Entity createWeapon(Registry& registry, Parent parent, Entity projectile);
}

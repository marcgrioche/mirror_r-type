/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** WeaponSystem - Handles weapon shooting logic for network game
*/

#pragma once

#include "Entity.hpp"
#include "Registry.hpp"
#include <cstdint>
#include <vector>

namespace WeaponSystem {
/**
 * Handle player weapon attack input
 * Creates projectiles when player shoots with proper frequency checking
 *
 * @param registry The game registry containing all entities and components
 * @param playerEntity The player entity attempting to shoot
 * @param playerId The player's network ID for projectile ownership tracking
 * @param newEntitiesThisTick Vector to track newly created entities for network synchronization
 * @param mouseX Mouse X coordinate for aiming (optional, default 0)
 * @param mouseY Mouse Y coordinate for aiming (optional, default 0)
 * @return true if a projectile was created, false otherwise
 */
bool handlePlayerAttack(
    Registry& registry,
    Entity playerEntity,
    uint32_t playerId,
    std::vector<Entity>& newEntitiesThisTick,
    float mouseX = 0.0f,
    float mouseY = 0.0f);

/**
 * Handle automatic enemy weapon attacks.
 * Iterates over all weapons whose parent entity is an Enemy and, if the
 * weapon frequency allows, spawns a projectile travelling toward players
 * (currently straight to the left).
 *
 * @param registry Game registry containing all entities and components
 * @param newEntitiesThisTick Vector to push newly created projectiles for network synchronization
 * @return true if at least one projectile was created this tick
 */
bool handleEnemyAttacks(
    Registry& registry,
    std::vector<Entity>& newEntitiesThisTick);
}

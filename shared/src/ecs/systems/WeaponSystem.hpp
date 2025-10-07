/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** WeaponSystem - Handles weapon shooting logic for network game
*/

#pragma once

#include "Registry.hpp"
#include "Entity.hpp"
#include <cstdint>
#include <vector>

namespace WeaponSystem {
    /**
     * Handle player weapon attack input
     * Creates projectiles when player shoots with proper frequency checking
     * 
     * @param registry The game registry
     * @param playerEntity The player entity attempting to shoot
     * @param playerId The player's network ID
     * @param newEntitiesThisTick Vector to track newly created entities for sync
     * @return true if a projectile was created, false otherwise
     */
    bool handlePlayerAttack(
        Registry& registry,
        Entity playerEntity,
        uint32_t playerId,
        std::vector<Entity>& newEntitiesThisTick
    );

    /**
     * Handle automatic enemy weapon attacks.
     * Iterates over all weapons whose parent entity is an Enemy and, if the
     * weapon frequency allows, spawns a projectile travelling toward players
     * (currently straight to the left).
     *
     * @param registry Game registry
     * @param newEntitiesThisTick Vector to push newly created projectiles for sync
     * @return true if at least one projectile was created this tick
     */
    bool handleEnemyAttacks(
        Registry& registry,
        std::vector<Entity>& newEntitiesThisTick
    );
}

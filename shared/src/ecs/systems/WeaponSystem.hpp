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
}

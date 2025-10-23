/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Tags - Component tags for entity categorization and system filtering
*/

#pragma once

/**
 * @brief Tag component identifying player-controlled entities
 * @param playerId representing the player Id on the server
 */
struct PlayerTag {
    uint32_t playerId;
};

/**
 * @brief Empty tag component identifying enemy entities
 */
struct EnemyTag { };

/**
 * @brief Empty tag component identifying platform/terrain entities
 */
struct PlatformTag { };

/**
 * @brief Empty tag component identifying projectile entities
 */
struct ProjectileTag { };

/**
 * @brief Empty tag component identifying weapon entities
 */
struct WeaponTag { };

/**
 * @brief Empty tag component identifying power-up entities
 */
struct PowerUpTag { };

/**
 * @brief Empty tag component identifying boss entities
 */
struct BossTag { };

/**
 * @brief Empty tag component identifying UI button entities
 */
struct ButtonTag { };

/**
 * @brief Empty tag component identifying UI text input entities
 */
struct TextBoxTag { };

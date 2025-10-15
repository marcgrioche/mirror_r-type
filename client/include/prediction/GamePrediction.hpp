/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game Prediction Header - Client-side prediction for enemy projectiles
*/

#pragma once

#include "../../../shared/src/ecs/Registry.hpp"

namespace GamePrediction {

/**
 * @brief Predicts enemy projectile spawning on the client side
 * This reduces network traffic by locally creating projectiles that enemies fire
 * based on their weapon frequencies, instead of waiting for server messages.
 *
 * @param registry The game registry containing all entities
 */
void predictEnemyProjectiles(Registry& registry);

} // namespace GamePrediction

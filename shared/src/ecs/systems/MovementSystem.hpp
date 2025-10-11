/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** MovementSystem - Handle entity movement
*/

#pragma once
#include "Registry.hpp"

/**
 * @brief System that updates entity positions based on their velocity components
 * Applies velocity to position for all entities that have both Position and Velocity components
 * @param registry The ECS registry containing all game entities and components
 * @param deltaTime Time elapsed since the last frame in seconds
 * @return Number of entities that were moved during this update
 */
int movementSystem(Registry& registry, float deltaTime);

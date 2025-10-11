/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** HealthSystem - Manages entity health and death states
*/

#pragma once
#include "Registry.hpp"

/**
 * @brief System that manages entity health and handles death conditions
 * Checks entities with Health components and marks them as dead when health reaches zero
 * @param registry The ECS registry containing all game entities and components
 */
void healthSystem(Registry& registry);

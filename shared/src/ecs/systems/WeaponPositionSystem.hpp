/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** WeaponPositionSystem - Updates weapon positions relative to their parent entities
*/

#pragma once
#include "Registry.hpp"

/**
 * @brief System that updates weapon positions to follow their parent entities
 * Ensures weapons are positioned correctly relative to the entities that carry them
 * @param registry The ECS registry containing all game entities and components
 */
void weaponPositionSystem(Registry& registry);

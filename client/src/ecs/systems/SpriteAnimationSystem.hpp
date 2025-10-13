/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SpriteAnimationSystem
*/

#pragma once
#include "Registry.hpp"

/**
 * @brief Updates sprite animations for all animated entities
 * @param registry The ECS registry containing animated sprite entities
 * @param deltaTime Time elapsed since last update for animation timing
 */
void spriteAnimationSystem(Registry& registry, float deltaTime);

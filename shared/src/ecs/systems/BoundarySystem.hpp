/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** BoundarySystem - Handles screen boundary checking
*/

#pragma once
#include "Registry.hpp"

/**
 * @brief System that checks and handles entities that go beyond screen boundaries
 * Entities that move outside the screen boundaries are typically destroyed or repositioned
 * @param registry The ECS registry containing all game entities and components
 */
void boundarySystem(Registry& registry);

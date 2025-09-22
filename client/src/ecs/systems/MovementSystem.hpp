/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** MovementSystem - Handle entity movement
*/

#pragma once
#include "ecs/Registry.hpp"

// Movement system - updates entity positions based on velocity
void movementSystem(Registry& registry, float deltaTime);

// Boundary wrapping system - keeps entities within screen bounds
void boundarySystem(Registry& registry);

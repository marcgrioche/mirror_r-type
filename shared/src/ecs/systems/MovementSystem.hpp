/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** MovementSystem - Handle entity movement
*/

#pragma once
#include "Registry.hpp"
#include <optional>

// Movement system - updates entity positions based on velocity
void movementSystem(std::shared_ptr<Registry> registry, float deltaTime, std::optional<Entity> t_onlyEntity = std::nullopt);

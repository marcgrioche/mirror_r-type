/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** GravitySystem
*/

#pragma once
#include <optional>

#include "Registry.hpp"

void gravitySystem(std::shared_ptr<Registry> registry, float deltaTime, std::optional<Entity> t_onlyEntity = std::nullopt);

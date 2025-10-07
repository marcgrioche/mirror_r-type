/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** enemyMovement
*/

#pragma once
#include "Entity.hpp"
#include "Registry.hpp"
#include "components/AllComponents.hpp"

void enemyMovement(std::shared_ptr<Registry> registry, float deltaTime);

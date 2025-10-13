/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** HandlePlayerInputs
*/

#pragma once
#include "Entity.hpp"
#include "Registry.hpp"
#include "managers/InputManager.hpp"

/**
 * @brief Handles player input events and updates the game state accordingly
 * @param _inputs The input manager containing current input states
 * @param registry The ECS registry containing game entities
 */
void handlePlayerInputs(InputManager& _inputs, Registry& registry);

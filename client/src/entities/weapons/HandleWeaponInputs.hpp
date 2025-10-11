/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** handleWeaponInputs
*/

#pragma once
#include "Registry.hpp"
#include "managers/InputManager.hpp"

/**
 * @brief Handles weapon-related input events and updates weapon systems accordingly
 * @param _inputs The input manager containing current input states
 * @param registry The ECS registry containing game entities
 */
void handleWeaponInputs(InputManager& _inputs, Registry& registry);

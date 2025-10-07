/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** HandlePlayerInputs
*/

#pragma once
#include "Entity.hpp"
#include "Registry.hpp"
#include "components/AllComponents.hpp"
#include "managers/InputManager.hpp"

void handlePlayerInputs(InputManager& _inputs, std::shared_ptr<Registry> registry);
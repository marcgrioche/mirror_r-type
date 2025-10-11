/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** HandlePlayerInputs
*/

#pragma once
#include "Entity.hpp"
#include "Registry.hpp"

/**
 * @brief System that handles platform movement in the ECS registry
 * @param registry The ECS registry containing platform entities
 */
void movePlatform(Registry& registry);

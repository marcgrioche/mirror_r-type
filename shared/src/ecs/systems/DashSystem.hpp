/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** DashSystem - Handles player dash mechanics
*/

#pragma once
#include "Registry.hpp"
#include "components/Dash.hpp"
#include "components/RigidBody.hpp"
#include "components/Velocity.hpp"

/**
 * @brief System that manages player dash abilities and cooldowns
 * Updates dash timers and applies dash movement to players with active dash components
 * @param registry The ECS registry containing all game entities and components
 * @param deltaTime Time elapsed since the last frame in seconds
 */
void dashSystem(Registry& registry, float deltaTime);

void changeDashComponentProperties(
    Dash& t_dash,
    Velocity& t_velocity,
    RigidBody& t_rigidBody,
    float t_deltaTime);

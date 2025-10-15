/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** GravitySystem - Applies gravitational forces to entities
*/

#pragma once
#include "Registry.hpp"
#include "components/RigidBody.hpp"
#include "components/Velocity.hpp"

/**
 * @brief System that applies gravitational acceleration to entities with RigidBody components
 * Entities affected by gravity will accelerate downward unless they are on a platform
 * @param registry The ECS registry containing all game entities and components
 * @param deltaTime Time elapsed since the last frame in seconds
 */

void rigidBodySystem(Registry& registry, float deltaTime);

void changeRigidBodyComponentProperties(
    RigidBody& t_rigidBody,
    Velocity& t_velocity,
    float t_deltaTime,
    bool t_isPlayer);

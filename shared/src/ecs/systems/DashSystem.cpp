/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** DashSystem - Handles player dash mechanics
*/

#include "DashSystem.hpp"
#include "Dash.hpp"
#include "FrequencyUtils.hpp"
#include <iostream>

void changeDashComponentProperties(
    Dash& t_dash,
    Velocity& t_velocity,
    RigidBody& t_rigidBody,
    const float t_deltaTime)
{
    if (t_dash.isDashing) {
        t_dash.remaining -= t_deltaTime;

        t_velocity.v.x = t_dash.dashSpeed * t_dash.direction.x;
        t_velocity.v.y = t_dash.dashSpeed * t_dash.direction.y;

        if (t_dash.remaining <= 0.0f) {
            t_rigidBody.active = true; // reactivate rigid body
            t_dash.isDashing = false;
            t_dash.remaining = 0.0f;
            FrequencyUtils::reset(t_dash.cooldown);
        }
    }
}

void dashSystem(Registry& registry, float deltaTime)
{
    auto view = registry.view<Dash, Velocity, RigidBody>();

    for (auto [dash, velocity, rigidBody] : view) {
        changeDashComponentProperties(dash, velocity, rigidBody, deltaTime);
    }
}

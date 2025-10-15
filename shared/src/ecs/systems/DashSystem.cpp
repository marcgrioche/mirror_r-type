/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** DashSystem - Handles player dash mechanics
*/

#include "DashSystem.hpp"
#include "components/Dash.hpp"
#include "components/Velocity.hpp"
#include "Dash.hpp"
#include "FrequencyUtils.hpp"
#include <iostream>
#include "components/RigidBody.hpp"

void dashSystem(Registry& registry, float deltaTime)
{
    auto view = registry.view<Dash, Velocity, RigidBody>();

    for (auto [dash, velocity, rigidBody] : view) {
        if (dash.isDashing) {
            dash.remaining -= deltaTime;

            velocity.v.x = dash.dashSpeed * dash.direction.x;
            velocity.v.y = dash.dashSpeed * dash.direction.y;

            if (dash.remaining <= 0.0f) {
                rigidBody.active = true; //reactivate rigid body
                dash.isDashing = false;
                dash.remaining = 0.0f;
                FrequencyUtils::reset(dash.cooldown);
            }
        }
    }
}

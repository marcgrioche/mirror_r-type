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

void dashSystem(Registry& registry, float deltaTime)
{
    auto view = registry.view<Dash, Velocity>();

    for (auto [dash, velocity] : view) {
        if (dash.isDashing) {
            dash.remaining -= deltaTime;

            velocity.dx = dash.dashSpeed * dash.direction;
            velocity.dy = 0.0f;

            if (dash.remaining <= 0.0f) {
                dash.isDashing = false;
                dash.remaining = 0.0f;
                FrequencyUtils::reset(dash.cooldown);
                velocity.dx = 0.0f;
            }
        }
    }
}

/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** GravitySystem
*/

#include "GravitySystem.hpp"
#include "components/Velocity.hpp"
#include "components/Health.hpp"
#include "components/Tags.hpp"
#include "components/Dash.hpp"
#include "Config.hpp"

void gravitySystem(Registry& registry, float deltaTime)
{
    auto view = registry.view<PlayerTag, Velocity, Health, Dash>();

    for (auto&& [player, vel, health, dash] : view) {
        if (health.hp <= 0 || dash.isDashing) {
            continue;
        }

        if (vel.dy < 0.f) {
            vel.dy += GRAVITY_UP * deltaTime;
        } else {
            vel.dy += GRAVITY_DOWN * deltaTime;
        }

        if (vel.dy > MAX_FALL) {
            vel.dy = MAX_FALL;
        }
    }
}

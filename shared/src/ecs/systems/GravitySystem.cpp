/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** GravitySystem
*/

#include "GravitySystem.hpp"
#include "components/Velocity.hpp"
#include "components/Jump.hpp"
#include "components/Health.hpp"
#include "components/Tags.hpp"

void gravitySystem(Registry& registry, float deltaTime)
{
    auto view = registry.view<PlayerTag, Velocity, Jump, Health>();

    for (auto&& [player, vel, jump, health] : view) {
        if (health.hp <= 0) {
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

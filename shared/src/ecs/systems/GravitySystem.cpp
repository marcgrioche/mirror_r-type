/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** GravitySystem
*/

#include "GravitySystem.hpp"
#include "components/Jump.hpp"
#include "components/Tags.hpp"
#include "components/Velocity.hpp"

void gravitySystem(std::shared_ptr<Registry> registry, float deltaTime, std::optional<Entity> t_onlyEntity)
{
    auto view = registry->view<PlayerTag, Velocity, Jump>();

    if (t_onlyEntity.has_value()) {
        auto& entityVelocity = registry->get<Velocity>(t_onlyEntity.value());

        if (entityVelocity.dy < 0.f) {
            entityVelocity.dy += GRAVITY_UP * deltaTime;
        } else {
            entityVelocity.dy += GRAVITY_DOWN * deltaTime;
        }

        if (entityVelocity.dy > MAX_FALL) {
            entityVelocity.dy = MAX_FALL;
        }
    }
    for (auto&& [player, vel, jump] : view) {
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

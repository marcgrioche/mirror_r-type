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

void enemyMovement(Registry& registry, float deltaTime)
{
    auto view = registry.view<EnemyTag, Velocity>();

    for (auto&& [enemy, vel] : view) {
        vel.dx = -50.0;
    }
}
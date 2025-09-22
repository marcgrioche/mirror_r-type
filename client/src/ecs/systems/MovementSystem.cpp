/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** MovementSystem implementation
*/

#include "MovementSystem.hpp"
#include "ecs/Components.hpp"

void movementSystem(Registry& registry, float deltaTime)
{
    auto view = registry.view<Position, Velocity>();
    for (auto&& [pos, vel] : view) {
        pos.x += vel.dx * deltaTime;
        pos.y += vel.dy * deltaTime;
    }
}

void boundarySystem(Registry& registry)
{
    auto& ps = registry.get_or_create_storage<Position>();
    for (size_t i = 0; i < ps.dense_size(); ++i) {
        auto& pos = ps.get_by_dense_index(i);

        // Wrap around screen edges
        if (pos.x < -5)
            pos.x = 35;
        if (pos.x > 35)
            pos.x = -5;
        if (pos.y < -5)
            pos.y = 25;
        if (pos.y > 25)
            pos.y = -5;
    }
}

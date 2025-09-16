/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** main
*/

#include "ecs/Components.hpp"
#include "ecs/Registry.hpp"
#include <iostream>

int main()
{
    Registry registry;

    Entity e1 = registry.create_entity();
    Entity e2 = registry.create_entity();
    Entity e3 = registry.create_entity();

    registry.emplace<Position>(e1, 0.0f, 0.0f);
    registry.emplace<Velocity>(e1, 1.0f, 1.0f);

    registry.emplace<Position>(e2, 10.0f, 5.0f);

    registry.emplace<Position>(e3, 2.0f, 2.0f);
    registry.emplace<Velocity>(e3, -0.5f, 0.25f);

    for (int frame = 0; frame < 5; ++frame) {
        auto view = registry.view<Position, Velocity>();
        for (auto&& [pos, vel] : view) {
            pos.x += vel.dx;
            pos.y += vel.dy;
        }

        auto& ps = registry.get_or_create_storage<Position>();
        for (size_t i = 0; i < ps.dense_size(); ++i) {
            auto ent = ps.dense_entities_ref()[i];
            auto p = ps.get_by_dense_index(i);
            std::cout << ent << " Position(" << p.x << "," << p.y << ")\n";
        }
        std::cout << "----\n";
    }

    registry.kill_entity(e1);
    Entity e4 = registry.create_entity();
    std::cout << "Created e4 = " << e4 << "\n";

    return 0;
}

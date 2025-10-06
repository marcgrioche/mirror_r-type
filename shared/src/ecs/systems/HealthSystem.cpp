/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** GravitySystem
*/

#include "GravitySystem.hpp"
#include "components/Tags.hpp"
#include "components/Health.hpp"
#include <vector>

void healthSystem(Registry& registry)
{
    auto view = registry.view<Health>();

    // Collect entities to remove to avoid invalidating iterators during iteration
    std::vector<Entity> toKill;
    for (auto it = view.begin(); it != view.end(); ++it) {
        auto [e, hp] = it.entity_and_components();
        if (hp.hp <= 0) {
            toKill.push_back(e);
        }
    }

    for (const auto& e : toKill) {
        registry.kill_entity(e);
    }
}

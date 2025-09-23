/*
** ProjectileSystem.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Sep 23 1:13:29 PM 2025 jojo
** Last update Wed Sep 23 1:54:45 PM 2025 jojo
*/

#include "ProjectileSystem.hpp"

void projectileSystem(Registry& registry, float deltaTime)
{
    auto view = registry.view<Projectile, Lifetime>();

    for (auto it = view.begin(); it != view.end();) {
        auto [tag, lifetime] = *it;

        lifetime.value -= deltaTime;
        if (lifetime.value <= 0.0f) {
            // Entity entity_to_kill = it.entity();
            ++it;
            // registry.kill_entity(entity_to_kill);
        } else {
            ++it;
        }
    }
}

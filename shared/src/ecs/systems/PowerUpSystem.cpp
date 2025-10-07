/*
** PowerUpSystem.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 2:07:20 PM 2025 jojo
** Last update Wed Oct 7 2:07:45 PM 2025 jojo
*/

#include "PowerUpSystem.hpp"

void PowerUpSystem(Registry& registry, float deltaTime)
{
    auto view = registry.view<PowerUpTag, Lifetime>();

    for (auto it = view.begin(); it != view.end();) {
        auto [tag, lifetime] = *it;

        lifetime.value -= deltaTime;
        if (lifetime.value <= 0.0f) {
            Entity entity_to_kill = it.entity();
            ++it;
            registry.kill_entity(entity_to_kill);
        } else {
            ++it;
        }
    }
}

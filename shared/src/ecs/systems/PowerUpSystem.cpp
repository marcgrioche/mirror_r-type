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

void powerUpSystem(Registry& registry, float deltaTime)
{
    auto view = registry.view<PowerUpTag>();

    for (auto it = view.begin(); it != view.end();) {
        Entity entity = it.entity();
        ++it;
        if (registry.has<Lifetime>(entity)) {
            auto& lifetime = registry.get<Lifetime>(entity);
            lifetime.value -= deltaTime;
            if (lifetime.value <= 0.0f) {
                registry.kill_entity(entity);
            }
        }
    }
}

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
#include "components/Lifetime.hpp"
#include "components/Tags.hpp"

void powerUpSystem(Registry& registry, float deltaTime)
{
    auto view = registry.view<PowerUpTag>();
    std::vector<Entity> toKill;

    for (auto it = view.begin(); it != view.end(); ++it) {
        Entity entity = it.entity();
        if (registry.has<Lifetime>(entity)) {
            auto& lifetime = registry.get<Lifetime>(entity);
            lifetime.value -= deltaTime;
            if (lifetime.value <= 0.0f) {
                toKill.push_back(entity);
            }
        }
    }

    for (auto entity : toKill) {
        registry.kill_entity(entity);
    }
}

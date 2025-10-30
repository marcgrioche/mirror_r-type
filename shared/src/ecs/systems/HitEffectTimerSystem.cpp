/*
** HitEffectTimerSystem.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 29 6:23:43 PM 2025 jojo
** Last update Thu Oct 29 6:47:01 PM 2025 jojo
*/

#include "HitEffectTimerSystem.hpp"
#include "components/Health.hpp"

void hitEffectTimerSystem(Registry& registry, float deltaTime)
{
    auto view = registry.view<Health>();

    for (auto it = view.begin(); it != view.end(); ++it) {
        auto [e, hp] = it.entity_and_components();
        if (hp.hitTime > 0) {
            std::cout << hp.hitTime << std::endl;
            hp.hitTime -= deltaTime;
        } else {
            hp.hit = 0;
            hp.hitTime = 0.0f;
        }
    }
}
/*
** hitEffectSystem.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 29 6:02:32 PM 2025 jojo
** Last update Thu Oct 29 7:16:39 PM 2025 jojo
*/

#include "HitEffectSystem.hpp"
#include "components/Health.hpp"
#include "components/Sprite.hpp"

void hitEffectSystem(Registry& registry)
{
    auto view = registry.view<Health, Sprite>();

    for (auto it = view.begin(); it != view.end(); ++it) {
        auto [e, hp, spr] = it.entity_and_components();
        if (hp.hit == 1) {
            spr.color = { 255, 255, static_cast<uint8_t>(spr.color.g - 10), static_cast<uint8_t>(spr.color.b - 10) };
        } else {
            spr.color = { 255, 255, 255, 255 };
        }
    }
}
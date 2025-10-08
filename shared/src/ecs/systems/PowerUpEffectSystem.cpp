/*
** PowerUpEffectSystem.cpp for mirror_r-type
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 7 5:15:00 PM 2025 jojo
** Last update Wed Oct 7 5:15:00 PM 2025 jojo
*/

#include "PowerUpEffectSystem.hpp"
#include "components/PowerUp.hpp"
#include "components/Damage.hpp"
#include "components/Tags.hpp"
#include "components/Parent.hpp"
#include <iostream>

void powerUpEffectSystem(Registry& registry, float deltaTime)
{
    auto playerView = registry.view<PlayerTag>();
    for (auto it = playerView.begin(); it != playerView.end(); ++it) {
        Entity entity = it.entity();
        if (registry.has<PowerUp>(entity)) {
            auto& powerUp = registry.get<PowerUp>(entity);
            if (powerUp.remaining_time > 0.0f) {
                powerUp.remaining_time -= deltaTime;

                if (powerUp.remaining_time <= 0.0f) {
                    powerUp.is_power = false;
                    powerUp.remaining_time = 0.0f;
                    std::cout << "Power-up effect expired for player" << std::endl;
                }
            }
        }
    }

    auto projectileView = registry.view<ProjectileTag>();
    for (auto it = projectileView.begin(); it != projectileView.end(); ++it) {
        Entity entity = it.entity();
        if (registry.has<Parent>(entity) && registry.has<Damage>(entity)) {
            auto& parent = registry.get<Parent>(entity);
            if (registry.has<PowerUp>(parent.parent)) {
                auto& playerPowerUp = registry.get<PowerUp>(parent.parent);
                if (playerPowerUp.is_power && playerPowerUp.type == PowerUpType::DAMAGE_BOOST) {
                    auto& damage = registry.get<Damage>(entity);
                    damage.value *= 2;
                }
            }
        }
    }
}

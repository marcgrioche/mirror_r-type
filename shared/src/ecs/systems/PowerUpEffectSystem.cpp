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
#include "components/Frequency.hpp"

void powerUpEffectSystem(Registry& registry, float deltaTime)
{
    auto playerView = registry.view<PlayerTag>();
    for (auto it = playerView.begin(); it != playerView.end(); ++it) {
        Entity entity = it.entity();
        if (registry.has<PowerUp>(entity)) {
            auto& powerUp = registry.get<PowerUp>(entity);
            if (powerUp.remaining_time > 0.0f) {
                powerUp.remaining_time -= deltaTime;

                // If this is a FIRE_RATE power-up and not applied yet, apply to player's weapons
                if (powerUp.type == PowerUpType::FIRE_RATE && !powerUp.effect_applied) {
                    // find player's weapons and reduce their frequency
                    auto weaponView = registry.view<WeaponTag, Parent, Frequency>();
                    for (auto wit = weaponView.begin(); wit != weaponView.end(); ++wit) {
                        Entity w = wit.entity();
                        auto& parent = registry.get<Parent>(w);
                        if (parent.parent.id == entity.id && registry.has<Frequency>(w)) {
                            auto& freq = registry.get<Frequency>(w);
                            freq.frequency = static_cast<double>(freq.frequency * powerUp.fire_rate_multiplier);
                        }
                    }
                    powerUp.effect_applied = true;
                }

                if (powerUp.remaining_time <= 0.0f) {
                    // Expire effect and revert any applied modifications
                    if (powerUp.type == PowerUpType::FIRE_RATE && powerUp.effect_applied) {
                        auto weaponView = registry.view<WeaponTag, Parent, Frequency>();
                        for (auto wit = weaponView.begin(); wit != weaponView.end(); ++wit) {
                            Entity w = wit.entity();
                            auto& parent = registry.get<Parent>(w);
                            if (parent.parent.id == entity.id && registry.has<Frequency>(w)) {
                                auto& freq = registry.get<Frequency>(w);
                                // revert by dividing by multiplier (guard against zero)
                                if (powerUp.fire_rate_multiplier != 0.0f)
                                    freq.frequency = static_cast<double>(freq.frequency / powerUp.fire_rate_multiplier);
                            }
                        }
                        powerUp.effect_applied = false;
                    }
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

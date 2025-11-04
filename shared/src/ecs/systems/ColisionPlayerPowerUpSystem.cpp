/*
** ColisionPlayerPowerUpSystem.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 1:56:55 PM 2025 jojo
** Last update Wed Oct 7 2:06:13 PM 2025 jojo
*/

#include "ColisionPlayerPowerUpSystem.hpp"
#include "components/Position.hpp"
#include "components/Hitbox.hpp"
#include "components/Health.hpp"
#include "components/PowerUp.hpp"
#include "components/Lifetime.hpp"
#include "components/Tags.hpp"
#include "components/componentutils/HitboxUtils.hpp"
#include <iostream>
#include <algorithm>

void collisionPlayerPowerUpSystem(Registry& registry, float)
{
    auto powerView = registry.view<PowerUpTag>();
    auto playerView = registry.view<PlayerTag>();

    for (auto it = powerView.begin(); it != powerView.end(); ++it) {
        Entity powerE = it.entity();

        if (!registry.has<Position>(powerE) || !registry.has<Hitbox>(powerE))
            continue;

        for (auto pIt = playerView.begin(); pIt != playerView.end(); ++pIt) {
            Entity plE = pIt.entity();
            if (!registry.has<Position>(plE) || !registry.has<Hitbox>(plE))
                continue;

            if (entities_collide(registry, powerE, plE)) {
                auto& powerUpData = registry.get<PowerUp>(powerE);

                if (powerUpData.type == PowerUpType::HEAL) {
                    if (registry.has<Health>(plE)) {
                        auto& health = registry.get<Health>(plE);
                        health.hp = (std::min)(health.hp + 20, 100);
                        std::cout << "Player healed! Health: " << health.hp << std::endl;
                    }
                } else if (powerUpData.type == PowerUpType::DAMAGE_BOOST) {
                    std::cout << "Player got damage boost!" << std::endl;
                } else if (powerUpData.type == PowerUpType::FIRE_RATE) {
                    std::cout << "Player got fire-rate boost!" << std::endl;
                    // set default multiplier on the power-up data if not already set
                    if (powerUpData.effect_duration <= 0.0f) {
                        // give a sensible duration if none provided
                        powerUpData.effect_duration = 10.0f;
                    }
                    // We'll use 0.5 to make frequency half (twice the fire rate)
                    // store multiplier in the dropped power-up so it's transferred to the player
                    powerUpData.fire_rate_multiplier = 0.5f;
                }

                if (registry.has<PowerUp>(plE)) {
                    auto& playerPowerUp = registry.get<PowerUp>(plE);
                    playerPowerUp.is_power = true;
                    playerPowerUp.type = powerUpData.type;
                    playerPowerUp.remaining_time = powerUpData.effect_duration;
                    // Transfer any power-up specific parameters (e.g. fire rate multiplier)
                    if (powerUpData.type == PowerUpType::FIRE_RATE) {
                        playerPowerUp.fire_rate_multiplier = powerUpData.fire_rate_multiplier > 0.0f ? powerUpData.fire_rate_multiplier : 0.5f;
                        playerPowerUp.effect_applied = false;
                    }
                    if (powerUpData.type == PowerUpType::DAMAGE_BOOST) {
                        playerPowerUp.effect_applied = false;
                    }
                }

                if (registry.has<Lifetime>(powerE)) {
                    Lifetime& time = registry.get<Lifetime>(powerE);
                    time.value = 0.0f;
                }
            }
        }
    }
}

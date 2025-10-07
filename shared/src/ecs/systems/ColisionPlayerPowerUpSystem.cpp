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
                        health.hp = std::min(health.hp + 20, 100);
                        std::cout << "Player healed! Health: " << health.hp << std::endl;
                    }
                } else if (powerUpData.type == PowerUpType::DAMAGE_BOOST) {
                    std::cout << "Player got damage boost!" << std::endl;
                }

                if (registry.has<PowerUp>(plE)) {
                    auto& playerPowerUp = registry.get<PowerUp>(plE);
                    playerPowerUp.is_power = true;
                    playerPowerUp.type = powerUpData.type;
                    playerPowerUp.remaining_time = powerUpData.effect_duration;
                }

                Lifetime& time = registry.get<Lifetime>(powerE);
                time.value = 0.0f;
            }
        }
    }
}

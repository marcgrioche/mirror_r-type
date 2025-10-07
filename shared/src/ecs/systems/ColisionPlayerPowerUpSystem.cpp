/*
** ColisionPlayerPowerUpSystem.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 1:56:55 PM 2025 jojo
** Last update Wed Oct 7 1:59:48 PM 2025 jojo
*/

#include "ColisionPlayerPowerUpSystem.hpp"

void ColisionPlayerPowerUpSystem(Registry& registry, float)
{
    auto powerView = registry.view<PowerUpTag>();
    auto playerView = registry.view<Player>();

    for (auto it = powerView.begin(); it != powerView.end(); ++it) {
        Entity powerE = it.entity();

        // require Position + Hitbox on both sides
        if (!registry.has<Position>(powerE) || !registry.has<Hitbox>(powerE))
            continue;

        // check against all players
        for (auto pIt = playerView.begin(); pIt != playerView.end(); ++pIt) {
            Entity plE = pIt.entity();
            if (!registry.has<Position>(plE) || !registry.has<Hitbox>(plE))
                continue;

            if (entities_collide(registry, powerE, plE)) {
                // apply damage if player has Health and projectile has Damage
            }
        }
    }
}

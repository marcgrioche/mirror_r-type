/*
** CollisionPlayerProjectileSystem.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Sep 23 2:19:56 PM 2025 jojo
** Last update Wed Sep 23 2:53:21 PM 2025 jojo
*/

#include "CollisionPlayerProjectileSystem.hpp"
#include "Parent.hpp"

void collisionPlayerProjectileSystem(Registry& registry, float)
{
    auto projView = registry.view<ProjectileTag>();
    auto playerView = registry.view<PlayerTag>();

    for (auto it = projView.begin(); it != projView.end(); ++it) {
        Entity projE = it.entity();

        // require Position + Hitbox on both sides
        if (!registry.has<Position>(projE) || !registry.has<Hitbox>(projE))
            continue;

        // check against all players
        for (auto pIt = playerView.begin(); pIt != playerView.end(); ++pIt) {
            Entity plE = pIt.entity();
            if (!registry.has<Position>(plE) || !registry.has<Hitbox>(plE))
                continue;

            // avoid friendly fire by checking Parent
            if (registry.has<Parent>(projE)) {
                Entity weaponEntity = registry.get<Parent>(projE).parent;
                if (registry.has<Parent>(weaponEntity)) {
                    Entity ownerEntity = registry.get<Parent>(weaponEntity).parent;
                    if (ownerEntity.id == plE.id)
                        continue;
                }
            }

            if (entities_collide(registry, projE, plE)) {
                if (registry.has<Health>(plE) && registry.has<Damage>(projE)) {
                    Health& h = registry.get<Health>(plE);
                    float dmg = registry.get<Damage>(projE).value;
                    h.hp -= static_cast<int>(dmg);
                    if (h.hp <= 0) {
                        Dead& dead = registry.get<Dead>(plE);
                        dead.dead = true;
                    }
                    if (registry.has<Lifetime>(projE)) {
                        Lifetime& time = registry.get<Lifetime>(projE);
                        time.value = 0.0f;
                    }
                    std::cout << "[Collision] Player " << plE.id << " hit by projectile " << projE.id << " dmg=" << dmg << " hp=" << h.hp << "\n";
                }
            }
        }
    }
}
/*
** CollisionPlayerProjectileSystem.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Sep 23 2:19:56 PM 2025 jojo
** Last update Fri Oct 30 11:30:48 AM 2025 jojo
*/

#include "CollisionEnemyProjectileSystem.hpp"
#include "Parent.hpp"
#include "components/Damage.hpp"
#include "components/Dead.hpp"
#include "components/Health.hpp"
#include "components/Hitbox.hpp"
#include "components/Lifetime.hpp"
#include "components/Position.hpp"
#include "components/Tags.hpp"
#include "components/componentutils/HitboxUtils.hpp"
#include <iostream>

void collisionEnemyProjectileSystem(Registry& registry, float)
{
    auto projView = registry.view<ProjectileTag>();
    auto enemyView = registry.view<EnemyTag>();

    for (auto it = projView.begin(); it != projView.end(); ++it) {
        Entity projE = it.entity();

        if (!registry.has<Position>(projE) || !registry.has<Hitbox>(projE))
            continue;

        // check against all enemies
        for (auto eIT = enemyView.begin(); eIT != enemyView.end(); ++eIT) {
            Entity EnE = eIT.entity();
            if (!registry.has<Position>(EnE) || !registry.has<Hitbox>(EnE))
                continue;

            // avoid friendly fire by checking Parent
            if (registry.has<Parent>(projE)) {
                Entity weaponEntity = registry.get<Parent>(projE).parent;
                if (registry.has<Parent>(weaponEntity)) {
                    Entity ownerEntity = registry.get<Parent>(weaponEntity).parent;
                    if (ownerEntity.id == EnE.id || registry.has<EnemyTag>(ownerEntity))
                        continue;
                }
            }

            if (entities_collide(registry, projE, EnE)) {
                if (registry.has<Health>(EnE) && registry.has<Damage>(projE) && registry.has<Parent>(projE) /*&& registry.has<PlayerTag>(registry.get<Parent>(projE).parent)*/) {

                    Health& h = registry.get<Health>(EnE);
                    float dmg = registry.get<Damage>(projE).value;
                    h.hp -= static_cast<int>(dmg);
                    h.hit = 1;
                    h.hitTime = 0.2f;
                    std::cout << "[Collision] Enemy " << EnE.id << " hit by projectile " << projE.id << " dmg=" << dmg << " hp=" << h.hp << "\n";
                    if (h.hp <= 0) {
                        Dead& dead = registry.get<Dead>(EnE);
                        dead.dead = true;
                    }
                    if (registry.has<Lifetime>(projE)) {
                        Lifetime& time = registry.get<Lifetime>(projE);
                        time.value = 0.0f; // détruire le projectile
                    }
                }
            }
        }
    }
}
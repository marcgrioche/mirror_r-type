/*
** CollisionPlayerProjectileSystem.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Sep 23 2:19:56 PM 2025 jojo
** Last update Wed Sep 23 2:53:21 PM 2025 jojo
*/

#include "CollisionEnemyProjectileSystem.hpp"
#include "Parent.hpp"

void collisionEnemyProjectileSystem(Registry& registry, float)
{
    auto projView = registry.view<Projectile>();
    auto enemyView = registry.view<EnemyTag>();
    
    for (auto it = projView.begin(); it != projView.end(); ++it) {
        Entity projE = it.entity();
        
        // require Position + Hitbox on both sides
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
                    if (ownerEntity.id == EnE.id)
                    continue;
                }
            }
            
            if (entities_collide(registry, projE, EnE)) {
                // apply damage if enemy has Health and projectile has Damage
                if (registry.has<Health>(EnE) && registry.has<Damage>(projE)) {
                    std::cout << "Enemy collided Projectile\n";
                    Health& h = registry.get<Health>(EnE);
                    Lifetime& time = registry.get<Lifetime>(projE);
                    time.value = 0.0f;
                    float dmg = registry.get<Damage>(projE).value;
                    h.hp -= static_cast<int>(dmg);
                }
            }
        }
    }
}
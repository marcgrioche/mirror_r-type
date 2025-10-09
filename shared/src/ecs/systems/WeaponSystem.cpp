/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** WeaponSystem - Handles weapon shooting logic for network game
*/

#include "WeaponSystem.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/Damage.hpp"
#include "components/Hitbox.hpp"
#include "components/Lifetime.hpp"
#include "components/Tags.hpp"
#include "components/ProjectileType.hpp"
#include "components/Health.hpp"
#include "Parent.hpp"
#include "FrequencyUtils.hpp"
#include "../../entities/projectile/CreateProjectile.hpp"
#include "../../entities/enemies/CreateEnemy.hpp"
#include <iostream>


namespace WeaponSystem {

bool handlePlayerAttack(
    Registry& registry,
    Entity playerEntity,
    uint32_t playerId,
    std::vector<Entity>& newEntitiesThisTick)
{
    auto weaponView = registry.view<WeaponTag, Frequency, Parent, Damage>();
    
    for (auto weaponIt = weaponView.begin(); weaponIt != weaponView.end(); ++weaponIt) {
        auto [weaponTag, frequency, parent, damage] = *weaponIt;
        
        // Check if this weapon belongs to the player
        if (parent.parent.id != playerEntity.id) {
            continue;
        }
        
        if (!FrequencyUtils::shouldTrigger(frequency)) {
            return false;
        }
        
        if (!registry.has<Position>(playerEntity)) {
            std::cerr << "Player " << playerId << " has no Position component" << std::endl;
            return false;
        }
        const Position& playerPos = registry.get<Position>(playerEntity);
        // If the weapon has a ProjectileType, add this entity type in the registry
        if (registry.has<ProjectileType>(weaponIt.entity())) {
            const Entity tpl = registry.get<ProjectileType>(weaponIt.entity()).entity;
            const Position spawnPos { playerPos.x + 32.0f, playerPos.y + 16.0f };
            const Parent spawnParent { weaponIt.entity() };

            Entity spawned;
            if (registry.has<ProjectileTag>(tpl)) {
                Velocity vel = registry.has<Velocity>(tpl) ? Velocity {registry.get<Velocity>(tpl).dx, registry.get<Velocity>(tpl).dy} : Velocity { 300.0f, 0.0f };
                Hitbox hit = registry.has<Hitbox>(tpl) ? registry.get<Hitbox>(tpl) : Hitbox { 32.0f, 32.0f, 0.0f, 0.0f };
                Damage projDmg = registry.has<Damage>(tpl) ? registry.get<Damage>(tpl) : Damage { damage.value };
                Lifetime life = registry.has<Lifetime>(tpl) ? registry.get<Lifetime>(tpl): Lifetime { 5.0f };
                spawned = factories::createProjectile(registry, spawnPos, vel, projDmg, hit, spawnParent, life);
            } else if (registry.has<EnemyTag>(tpl)) {
                Health hp = registry.has<Health>(tpl) ? registry.get<Health>(tpl) : Health {10};
                Velocity vel = registry.has<Velocity>(tpl) ? registry.get<Velocity>(tpl) : Velocity { -50.0f, 0.0f };
                Hitbox hit = registry.has<Hitbox>(tpl) ? registry.get<Hitbox>(tpl) : Hitbox { 32.0f, 32.0f, 0.0f, 0.0f };
                spawned = factories::createEnemy(registry, spawnPos, hp, hit, vel);
                registry.emplace<Parent>(spawned, spawnParent);
            } else {
                spawned = factories::createProjectile(
                    registry,
                    spawnPos,
                    Velocity { 300.0f, 0.0f },
                    Damage { damage.value },
                    Hitbox { 32.0f, 32.0f, 0.0f, 0.0f },
                    spawnParent,
                    Lifetime { 5.0f }
                );
            }

            newEntitiesThisTick.push_back(spawned);
            FrequencyUtils::reset(frequency);
            return true;
            
            FrequencyUtils::reset(frequency);
            
            return true;
        }
        
    }
    
    return false;
}

bool handleEnemyAttacks(
    Registry& registry,
    std::vector<Entity>& newEntitiesThisTick)
{
    bool spawned = false;
    auto weaponView = registry.view<WeaponTag, Frequency, Parent, Damage>();

    for (auto weaponIt = weaponView.begin(); weaponIt != weaponView.end(); ++weaponIt) {
        auto [weaponTag, frequency, parent, damage] = *weaponIt;

        Entity owner = parent.parent;
        if (!registry.has<EnemyTag>(owner)) {
            continue; // only enemy owned weapons here
        }

        if (!FrequencyUtils::shouldTrigger(frequency)) {
            continue; // not ready yet
        }

        if (!registry.has<Position>(owner)) {
            continue; // cannot shoot without position
        }
        const Position& enemyPos = registry.get<Position>(owner);

        // Create entity based on the weapon's ProjectileType template by cloning it
        if (registry.has<ProjectileType>(weaponIt.entity())) {
            const Entity tpl = registry.get<ProjectileType>(weaponIt.entity()).entity;
            const Position spawnPos { enemyPos.x, enemyPos.y + rand() % (int)registry.get<Hitbox>(owner).height};
            const Parent spawnParent { weaponIt.entity() };

            Entity spawned;
            if (registry.has<ProjectileTag>(tpl)) {
                Velocity vel = registry.has<Velocity>(tpl) ? registry.get<Velocity>(tpl) : Velocity { -300.0f, 0.0f };
                Hitbox hit = registry.has<Hitbox>(tpl) ? registry.get<Hitbox>(tpl) : Hitbox { 32.0f, 32.0f, 0.0f, 0.0f };
                Damage projDmg = registry.has<Damage>(tpl) ? registry.get<Damage>(tpl) : Damage { damage.value };
                Lifetime life = registry.has<Lifetime>(tpl) ? registry.get<Lifetime>(tpl): Lifetime { 5.0f };
                spawned = factories::createProjectile(registry, spawnPos, vel, projDmg, hit, spawnParent, life);
            } else if (registry.has<EnemyTag>(tpl)) {
                Health hp = registry.has<Health>(tpl) ? registry.get<Health>(tpl) : Health {10};
                Velocity vel = registry.has<Velocity>(tpl) ? registry.get<Velocity>(tpl) : Velocity { -50.0f, 0.0f };
                Hitbox hit = registry.has<Hitbox>(tpl) ? registry.get<Hitbox>(tpl) : Hitbox { 32.0f, 32.0f, 0.0f, 0.0f };
                spawned = factories::createEnemy(registry, spawnPos, hp, hit, vel);
                registry.emplace<Parent>(spawned, spawnParent);
            } else {
                spawned = factories::createProjectile(
                    registry,
                    spawnPos,
                    Velocity { -300.0f, 0.0f },
                    Damage { damage.value },
                    Hitbox { 32.0f, 32.0f, 0.0f, 0.0f },
                    spawnParent,
                    Lifetime { 5.0f }
                );
            }

            newEntitiesThisTick.push_back(spawned);
            FrequencyUtils::reset(frequency);
            return true;
            
            FrequencyUtils::reset(frequency);
            
            return true;
        }

        // Enemies shoot towards the left (player side) for now.
        // registry.emplace<Position>(projectileEntity, enemyPos.x - 16.0f, enemyPos.y + 16.0f);
        // registry.emplace<Velocity>(projectileEntity, -300.0f, 0.0f);
        // registry.emplace<Damage>(projectileEntity, damage.value);
        // registry.emplace<Hitbox>(projectileEntity, 32.0f, 32.0f, 0.0f, 0.0f);
        // registry.emplace<Parent>(projectileEntity, Parent{weaponIt.entity()});
        // registry.emplace<Lifetime>(projectileEntity, 5.0f);
        // registry.emplace<ProjectileTag>(projectileEntity);

    }
    return spawned;
}

} // namespace WeaponSystem

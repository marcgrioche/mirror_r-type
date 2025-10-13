/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** WeaponSystem - Handles weapon shooting logic for network game
*/

#include "WeaponSystem.hpp"
#include "../../entities/enemies/CreateEnemy.hpp"
#include "../../entities/projectile/CreateProjectile.hpp"
#include "FrequencyUtils.hpp"
#include "Parent.hpp"
#include "components/Damage.hpp"
#include "components/Health.hpp"
#include "components/Hitbox.hpp"
#include "components/Lifetime.hpp"
#include "components/Position.hpp"
#include "components/ProjectileType.hpp"
#include "components/Tags.hpp"
#include "components/Velocity.hpp"
#include <algorithm>
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
        Entity weaponEntity = weaponIt.entity();

        if (!registry.has<Frequency>(weaponEntity) || !registry.has<Parent>(weaponEntity) || !registry.has<Damage>(weaponEntity)) {
            continue;
        }

        Frequency& frequency = registry.get<Frequency>(weaponEntity);
        Parent& parent = registry.get<Parent>(weaponEntity);
        Damage& damage = registry.get<Damage>(weaponEntity);

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
            const Position spawnPos { playerPos.x + registry.get<Hitbox>(playerEntity).width / 2, playerPos.y + registry.get<Hitbox>(playerEntity).height / 2 - registry.get<Hitbox>(tpl).height / 2 };
            const Parent spawnParent { weaponIt.entity() };

            Entity spawned;
            if (registry.has<ProjectileTag>(tpl)) {
                Velocity vel = registry.has<Velocity>(tpl) ? Velocity { registry.get<Velocity>(tpl).dx, registry.get<Velocity>(tpl).dy } : Velocity { 300.0f, 0.0f };
                Hitbox hit = registry.has<Hitbox>(tpl) ? registry.get<Hitbox>(tpl) : Hitbox { 32.0f, 32.0f, 0.0f, 0.0f };
                Damage projDmg = registry.has<Damage>(tpl) ? registry.get<Damage>(tpl) : Damage { damage.value };
                Lifetime life = registry.has<Lifetime>(tpl) ? registry.get<Lifetime>(tpl) : Lifetime { 5.0f };
                spawned = factories::createProjectile(registry, spawnPos, vel, projDmg, hit, spawnParent, life);
            } else if (registry.has<EnemyTag>(tpl)) {
                Health hp = registry.has<Health>(tpl) ? registry.get<Health>(tpl) : Health { 10 };
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
                    Lifetime { 5.0f });
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
    std::vector<Entity> weaponsToFire;
    auto weaponView = registry.view<WeaponTag, Frequency, Parent, Damage>();

    for (auto weaponIt = weaponView.begin(); weaponIt != weaponView.end(); ++weaponIt) {
        Entity weaponEntity = weaponIt.entity();

        if (!registry.has<Frequency>(weaponEntity) || !registry.has<Parent>(weaponEntity) || !registry.has<Damage>(weaponEntity)) {
            continue;
        }

        const Frequency& frequency = registry.get<Frequency>(weaponEntity);
        const Parent& parent = registry.get<Parent>(weaponEntity);

        Entity owner = parent.parent;
        if (!registry.has<EnemyTag>(owner)) {
            continue;
        }

        if (!FrequencyUtils::shouldTrigger(frequency)) {
            continue;
        }

        if (!registry.has<Position>(owner)) {
            continue;
        }

        weaponsToFire.push_back(weaponEntity);
    }

    for (Entity weaponEntity : weaponsToFire) {
        if (!registry.has<Parent>(weaponEntity) || !registry.has<Damage>(weaponEntity)) {
            continue;
        }

        const Parent& parent = registry.get<Parent>(weaponEntity);
        const Damage& damage = registry.get<Damage>(weaponEntity);
        Entity owner = parent.parent;

        if (!registry.has<Position>(owner)) {
            continue;
        }
        const Position& enemyPos = registry.get<Position>(owner);

        if (registry.has<ProjectileType>(weaponEntity)) {
            const Entity tpl = registry.get<ProjectileType>(weaponEntity).entity;
            const Hitbox& ownerHit = registry.get<Hitbox>(owner);
            const Hitbox& projHit = registry.get<Hitbox>(tpl);
            Position spawnPos { enemyPos.x, enemyPos.y + ownerHit.height / 2 - projHit.height / 2 };

            if (registry.has<BossTag>(owner)) {
                int maxOffset = static_cast<int>(std::max(0.0f, ownerHit.height - projHit.height * 2));
                int offset = maxOffset > 0 ? rand() % maxOffset : 0;
                spawnPos.y = enemyPos.y + offset;
            }
            const Parent spawnParent { weaponEntity };

            Entity spawned;
            if (registry.has<ProjectileTag>(tpl)) {
                Velocity vel = registry.has<Velocity>(tpl) ? registry.get<Velocity>(tpl) : Velocity { -300.0f, 0.0f };
                Hitbox hit = registry.has<Hitbox>(tpl) ? registry.get<Hitbox>(tpl) : Hitbox { 32.0f, 32.0f, 0.0f, 0.0f };
                Damage projDmg = registry.has<Damage>(tpl) ? registry.get<Damage>(tpl) : Damage { damage.value };
                Lifetime life = registry.has<Lifetime>(tpl) ? registry.get<Lifetime>(tpl) : Lifetime { 5.0f };
                spawned = factories::createProjectile(registry, spawnPos, vel, projDmg, hit, spawnParent, life);
            } else if (registry.has<EnemyTag>(tpl)) {
                Health hp = registry.has<Health>(tpl) ? registry.get<Health>(tpl) : Health { 10 };
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
                    Lifetime { 5.0f });
            }

            newEntitiesThisTick.push_back(spawned);

            if (registry.has<Frequency>(weaponEntity)) {
                Frequency& frequency = registry.get<Frequency>(weaponEntity);
                FrequencyUtils::reset(frequency);
            }
        }
    }

    return !weaponsToFire.empty();
}

} // namespace WeaponSystem

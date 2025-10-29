/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** BossAttackTriggerSystem - Triggers boss attack animation when a boss fires
*/

#include "BossAttackTriggerSystem.hpp"
#include "../../../../shared/src/ecs/components/Tags.hpp"
#include "../../../../shared/src/ecs/components/Parent.hpp"
#include "../../../../shared/src/ecs/components/IsAttacking.hpp"
#include <unordered_set>
#include <cstdint>
#include <iostream>

namespace {
// Keep track of projectiles we've already processed to only trigger on spawn
static std::unordered_set<uint64_t> s_seenProjectiles;

inline uint64_t makeKey(uint32_t id, uint32_t version)
{
    return (static_cast<uint64_t>(id) << 32) | static_cast<uint64_t>(version);
}
}

void bossAttackTriggerSystem(Registry& registry)
{
    // Prune entries for entities that no longer exist
    if (!s_seenProjectiles.empty()) {
        std::vector<uint64_t> toErase;
        toErase.reserve(s_seenProjectiles.size());
        for (uint64_t key : s_seenProjectiles) {
            uint32_t id = static_cast<uint32_t>(key >> 32);
            uint32_t ver = static_cast<uint32_t>(key & 0xFFFFFFFFu);
            if (registry.get_version(id) != ver) {
                toErase.push_back(key);
            }
        }
        for (auto k : toErase) s_seenProjectiles.erase(k);
    }

    auto projectileView = registry.view<ProjectileTag>();
    for (auto it = projectileView.begin(); it != projectileView.end(); ++it) {
        Entity proj = it.entity();
        const uint64_t key = makeKey(proj.id, proj.version);

        // Only react once per projectile
        if (s_seenProjectiles.find(key) != s_seenProjectiles.end())
            continue;

        s_seenProjectiles.insert(key);

        // Find weapon that spawned this projectile
        if (!registry.has<Parent>(proj))
            continue;
        const Parent& projParent = registry.get<Parent>(proj); // weapon entity
        Entity weapon = projParent.parent;

        // Find owner of the weapon (should be the boss)
        if (!registry.has<Parent>(weapon))
            continue;
        const Parent& weaponParent = registry.get<Parent>(weapon);
        Entity owner = weaponParent.parent;

        if (!registry.has<BossTag>(owner))
            continue;

        // Mark boss as attacking for one frame; SpriteAnimationSystem will switch ID and reset to idle immediately
        if (!registry.has<IsAttacking>(owner)) {
            registry.emplace<IsAttacking>(owner, 0);
        }

        auto& isAttacking = registry.get<IsAttacking>(owner);
        isAttacking.attacking = 1;

        // Optional debug
        // std::cout << "[CLIENT] BossAttackTriggerSystem: boss " << owner.id << ": attack triggered by projectile " << proj.id << "\n";
    }
}

/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** WeaponSystem - Handles weapon shooting logic for network game
*/

#include "WeaponSystem.hpp"
#include "components/AllComponents.hpp"
#include "Parent.hpp"
#include "FrequencyUtils.hpp"
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

        Entity projectileEntity = registry.create_entity();
        
        registry.emplace<Position>(projectileEntity, playerPos.x + 32.0f, playerPos.y + 16.0f);
        registry.emplace<Velocity>(projectileEntity, 500.0f, 0.0f);
        registry.emplace<Damage>(projectileEntity, damage.value);
        registry.emplace<Hitbox>(projectileEntity, 50.0f, 50.0f, 0.0f, 0.0f);
        registry.emplace<Parent>(projectileEntity, Parent{weaponIt.entity()});
        registry.emplace<Lifetime>(projectileEntity, 3.0f);
        registry.emplace<ProjectileTag>(projectileEntity);
        registry.emplace<OwnerId>(projectileEntity, static_cast<int>(playerId));
        
        // Add to new entities for synchronization
        newEntitiesThisTick.push_back(projectileEntity);
        
        FrequencyUtils::reset(frequency);
        
        return true;
    }
    
    return false;
}

} // namespace WeaponSystem

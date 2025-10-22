/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** WeaponRotationSystem - Updates weapon sprite rotation to aim at mouse cursor
*/

#include "WeaponRotationSystem.hpp"
#include "components/Sprite.hpp"
#include "components/SpriteManager.hpp"
#include "components/Tags.hpp"
#include "components/Parent.hpp"
#include "components/Position.hpp"
#include "components/Hitbox.hpp"
#include "components/WeaponRotation.hpp"
#include <cmath>

namespace WeaponRotationSystem {

void updateWeaponRotations(Registry& registry, float mouseX, float mouseY)
{
    auto weaponView = registry.view<WeaponTag, Parent>();

    for (auto it = weaponView.begin(); it != weaponView.end(); ++it) {
        Entity weaponEntity = it.entity();

        if (!registry.has<Parent>(weaponEntity)) {
            continue;
        }

        if (!registry.has<Sprite>(weaponEntity)) {
            SpriteManager::addWeaponSprite(registry, weaponEntity, 0.5f);
        }

        const Parent& parent = registry.get<Parent>(weaponEntity);
        Entity parentEntity = parent.parent;

        if (!registry.has<Position>(parentEntity) || !registry.has<Hitbox>(parentEntity)) {
            continue;
        }

        const Position& parentPos = registry.get<Position>(parentEntity);
        
        if (!registry.has<Position>(weaponEntity)) {
            registry.add<Position>(weaponEntity, Position { parentPos.v.x, parentPos.v.y });
        } else {
            Position& weaponPos = registry.get<Position>(weaponEntity);
            weaponPos.v.x = parentPos.v.x;
            weaponPos.v.y = parentPos.v.y;
        }

        if (!registry.has<PlayerTag>(parentEntity)) {
            continue;
        }

        const Hitbox& parentHitbox = registry.get<Hitbox>(parentEntity);
        Sprite& weaponSprite = registry.get<Sprite>(weaponEntity);

        float weaponCenterX = parentPos.v.x + parentHitbox.width / 2.0f;
        float weaponCenterY = parentPos.v.y + parentHitbox.height / 2.0f;

        float deltaX = mouseX - weaponCenterX;
        float deltaY = mouseY - weaponCenterY;

        float angleRadians = std::atan2(deltaY, deltaX);
        float angleDegrees = angleRadians * 180.0f / M_PI;

        weaponSprite.rotation = angleDegrees;
        
        if (!registry.has<WeaponRotation>(parentEntity)) {
            registry.add<WeaponRotation>(parentEntity, WeaponRotation { angleDegrees });
        } else {
            WeaponRotation& weaponRot = registry.get<WeaponRotation>(parentEntity);
            weaponRot.angle = angleDegrees;
        }
    }
}

void applyNetworkWeaponRotations(Registry& registry)
{
    auto playerView = registry.view<PlayerTag, WeaponRotation>();

    for (auto it = playerView.begin(); it != playerView.end(); ++it) {
        Entity playerEntity = it.entity();

        if (!registry.has<WeaponRotation>(playerEntity)) {
            continue;
        }

        const WeaponRotation& weaponRot = registry.get<WeaponRotation>(playerEntity);

        auto weaponView = registry.view<WeaponTag, Parent, Sprite>();
        for (auto weaponIt = weaponView.begin(); weaponIt != weaponView.end(); ++weaponIt) {
            Entity weaponEntity = weaponIt.entity();

            if (!registry.has<Parent>(weaponEntity) || !registry.has<Sprite>(weaponEntity)) {
                continue;
            }

            const Parent& parent = registry.get<Parent>(weaponEntity);
            if (parent.parent.id != playerEntity.id) {
                continue;
            }

            Sprite& weaponSprite = registry.get<Sprite>(weaponEntity);
            weaponSprite.rotation = weaponRot.angle;
        }
    }
}

}

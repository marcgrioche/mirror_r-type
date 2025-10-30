/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** WeaponRotationSystem - Rotates weapon sprites toward mouse cursor
*/

#include "WeaponRotationSystem.hpp"
#include "components/Hitbox.hpp"
#include "components/Parent.hpp"
#include "components/Position.hpp"
#include "components/Sprite.hpp"
#include "components/SpriteManager.hpp"
#include "components/Tags.hpp"
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

        const Parent& parent = registry.get<Parent>(weaponEntity);
        Entity ownerEntity = parent.parent;

        // Vérifier que c'est une arme de joueur
        if (!registry.has<PlayerTag>(ownerEntity)) {
            continue;
        }

        if (!registry.has<Position>(ownerEntity) || !registry.has<Hitbox>(ownerEntity)) {
            continue;
        }

        // Ajouter le sprite si nécessaire
        if (!registry.has<Sprite>(weaponEntity)) {
            SpriteManager::addWeaponSprite(registry, weaponEntity, 0.0f, 0.0f, 1.2f);
        }

        const Position& ownerPos = registry.get<Position>(ownerEntity);
        const Hitbox& ownerHitbox = registry.get<Hitbox>(ownerEntity);

        // Calculer le centre du joueur
        float centerX = ownerPos.v.x + ownerHitbox.width / 2.0f;
        float centerY = ownerPos.v.y + ownerHitbox.height / 2.0f;

        // Positionner l'arme au centre du joueur
        if (!registry.has<Position>(weaponEntity)) {
            registry.add<Position>(weaponEntity, Position{Vector2{centerX, centerY}});
        }
        Position& weaponPos = registry.get<Position>(weaponEntity);
        weaponPos.v.x = centerX;
        weaponPos.v.y = centerY;

        if (registry.has<PlayerTag>(ownerEntity)) {
            centerX += 100;
            centerY += 85;
        }

        // Calculer l'angle vers la souris
        float deltaX = mouseX - centerX;
        float deltaY = mouseY - centerY;
        float angleDegrees = atan2(deltaY, deltaX) * 180.0f / M_PI;

        // Appliquer la rotation au sprite
        if (registry.has<Sprite>(weaponEntity)) {
            Sprite& weaponSprite = registry.get<Sprite>(weaponEntity);
            weaponSprite.rotation = angleDegrees;
        }
    }
}

} // namespace WeaponRotationSystem

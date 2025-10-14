/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** CollisionSystem - Handle collisions between entities
*/

#include "CollisionSystem.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/Hitbox.hpp"
#include "components/PreviousPosition.hpp"
#include "components/Platform.hpp"
#include "components/Tags.hpp"
#include "components/componentutils/HitboxUtils.hpp"
#include <algorithm>
#include <iostream>
#include "components/RigidBody.hpp"

void collisionSystem(Registry& registry, float deltaTime)
{
    (void)deltaTime;
    auto bodiesView = registry.view<Position, Velocity, Hitbox, PreviousPosition, RigidBody>();

    auto platformView = registry.view<NoPassPlatform, Position, Hitbox, Velocity>();
    auto oneWayPlatformView = registry.view<BottomPassPlatform, Position, Hitbox, Velocity>();

    for (auto&& [pos, vel, hitbox, prevPos, rigidBody] : bodiesView) {
        Position originalPos = { prevPos.v.x, prevPos.v.y };
        rigidBody.isOnPlatform = false;

        for (auto&& [p, platformPos, platformHitbox, platformVel] : platformView) {
            if (aabb_overlap_world(pos, hitbox, platformPos, platformHitbox)) {
                resolvePlatformCollision(pos, vel, hitbox,
                    platformPos, platformHitbox, originalPos, platformVel, rigidBody.isOnPlatform);
            }
        }

        for (auto&& [p, platformPos, platformHitbox, platformVel] : oneWayPlatformView) {
            if (aabb_overlap_world(pos, hitbox, platformPos, platformHitbox)) {
                resolveOneWayPlatformCollision(pos, vel, hitbox,
                    platformPos, platformHitbox, originalPos, platformVel, rigidBody.isOnPlatform);
            }
        }
    }
}

void resolvePlatformCollision(Position& pos, Velocity& vel, const Hitbox& hitbox,
    const Position& platformPos, const Hitbox& platformHitbox,
    const Position& originalPos, const Velocity& platformVel, bool &isPlayerOnPlatform)
{
    (void)originalPos;
    float bodyLeft = pos.v.x + hitbox.offset_x;
    float bodyRight = bodyLeft + hitbox.width;
    float bodyTop = pos.v.y + hitbox.offset_y;
    float bodyBottom = bodyTop + hitbox.height;

    float platformLeft = platformPos.v.x + platformHitbox.offset_x;
    float platformRight = platformLeft + platformHitbox.width;
    float platformTop = platformPos.v.y + platformHitbox.offset_y;
    float platformBottom = platformTop + platformHitbox.height;

    float overlapLeft = bodyRight - platformLeft;
    float overlapRight = platformRight - bodyLeft;
    float overlapTop = bodyBottom - platformTop;
    float overlapBottom = platformBottom - bodyTop;

    // plus l'overlap est petit, plus ça veut dire que ça "vient" de toucher
    float minOverlapX = std::min(overlapLeft, overlapRight);
    float minOverlapY = std::min(overlapTop, overlapBottom);

    if (minOverlapX < minOverlapY) {
        if (overlapLeft < overlapRight) {
            pos.v.x = platformLeft - (hitbox.offset_x + hitbox.width);
        } else {
            pos.v.x = platformRight - hitbox.offset_x;
        }
        vel.v.x = platformVel.v.x;
        std::cout << "Horizontal collision resolved!" << std::endl;
    } else {
        if (overlapTop < overlapBottom) {
            pos.v.y = platformTop - (hitbox.offset_y + hitbox.height);
            if (vel.v.y >= 0) {
                vel.v.y = 0.0f;
                float inputVel = vel.v.x;
                vel.v.x = 2 * platformVel.v.x + inputVel;
                isPlayerOnPlatform = true;
            }
        } else {
            pos.v.y = platformBottom - hitbox.offset_y;
            if (vel.v.y < 0) {
                vel.v.y = 0.0f;
                std::cout << "Player hit ceiling!" << std::endl;
            }
        }
    }
}

void resolveOneWayPlatformCollision(Position& pos, Velocity& vel, const Hitbox& hitbox,
    const Position& platformPos, const Hitbox& platformHitbox,
    const Position& originalPos, const Velocity& platformVel, bool &isPlayerOnPlatform)
{
    if (vel.v.y <= 0) {
        return; // Player is not falling, ignore collision
    }

    // Check if body was above the platform in the previous frame
    float originalBodyBottom = originalPos.v.y + hitbox.offset_y + hitbox.height;
    float platformTop = platformPos.v.y + platformHitbox.offset_y;

    if (originalBodyBottom <= platformTop + 5.0f) {
        pos.v.y = platformTop - (hitbox.offset_y + hitbox.height);
        vel.v.y = 0.0f;
        float inputVel = vel.v.x;
        vel.v.x = 2 * platformVel.v.x + inputVel;
        isPlayerOnPlatform = true;
    }
}

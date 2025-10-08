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
    auto playerView = registry.view<Position, Velocity, Hitbox, PreviousPosition, RigidBody>();

    auto platformView = registry.view<NoPassPlatform, Position, Hitbox, Velocity>();
    auto oneWayPlatformView = registry.view<BottomPassPlatform, Position, Hitbox, Velocity>();

    for (auto&& [pos, vel, hitbox, prevPos, rigidBody] : playerView) {
        Position originalPos = { prevPos.x, prevPos.y };
        rigidBody.IsOnPlatform = false;

        for (auto&& [p, platformPos, platformHitbox, platformVel] : platformView) {
            if (aabb_overlap_world(pos, hitbox, platformPos, platformHitbox)) {
                resolvePlatformCollision(pos, vel, hitbox,
                    platformPos, platformHitbox, originalPos, platformVel, rigidBody.IsOnPlatform);
            }
        }

        for (auto&& [p, platformPos, platformHitbox, platformVel] : oneWayPlatformView) {
            if (aabb_overlap_world(pos, hitbox, platformPos, platformHitbox)) {
                resolveOneWayPlatformCollision(pos, vel, hitbox,
                    platformPos, platformHitbox, originalPos, platformVel, rigidBody.IsOnPlatform);
            }
        }
    }
}

void resolvePlatformCollision(Position& pos, Velocity& vel, const Hitbox& hitbox,
    const Position& platformPos, const Hitbox& platformHitbox,
    const Position& originalPos, const Velocity& platformVel, bool &isPlayerOnPlatform)
{
    (void)originalPos;
    float bodyLeft = pos.x + hitbox.offset_x;
    float bodyRight = bodyLeft + hitbox.width;
    float bodyTop = pos.y + hitbox.offset_y;
    float bodyBottom = bodyTop + hitbox.height;

    float platformLeft = platformPos.x + platformHitbox.offset_x;
    float platformRight = platformLeft + platformHitbox.width;
    float platformTop = platformPos.y + platformHitbox.offset_y;
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
            pos.x = platformLeft - (hitbox.offset_x + hitbox.width);
        } else {
            pos.x = platformRight - hitbox.offset_x;
        }
        vel.dx = platformVel.dx;
        std::cout << "Horizontal collision resolved!" << std::endl;
    } else {
        if (overlapTop < overlapBottom) {
            pos.y = platformTop - (hitbox.offset_y + hitbox.height);
            if (vel.dy >= 0) {
                vel.dy = 0.0f;
                float inputVel = vel.dx;
                vel.dx = 2 * platformVel.dx + inputVel;
                isPlayerOnPlatform = true;
            }
        } else {
            pos.y = platformBottom - hitbox.offset_y;
            if (vel.dy < 0) {
                vel.dy = 0.0f;
                std::cout << "Player hit ceiling!" << std::endl;
            }
        }
    }
}

void resolveOneWayPlatformCollision(Position& pos, Velocity& vel, const Hitbox& hitbox,
    const Position& platformPos, const Hitbox& platformHitbox,
    const Position& originalPos, const Velocity& platformVel, bool &isPlayerOnPlatform)
{
    if (vel.dy <= 0) {
        return; // Player is not falling, ignore collision
    }

    // Check if body was above the platform in the previous frame
    float originalBodyBottom = originalPos.y + hitbox.offset_y + hitbox.height;
    float platformTop = platformPos.y + platformHitbox.offset_y;

    if (originalBodyBottom <= platformTop + 5.0f) {
        pos.y = platformTop - (hitbox.offset_y + hitbox.height);
        vel.dy = 0.0f;
        float inputVel = vel.dx;
        vel.dx = 2 * platformVel.dx + inputVel;
        isPlayerOnPlatform = true;
    }
}

/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** CollisionSystem - Handle collisions between entities
*/

#include "CollisionSystem.hpp"
#include "components/AllComponents.hpp"
#include "components/componentutils/HitboxUtils.hpp"
#include <algorithm>
#include <iostream>

void collisionSystem(Registry& registry, float deltaTime)
{
    auto playerView = registry.view<PlayerTag, Position, Velocity, Hitbox, Jump, PreviousPosition>();

    auto platformView = registry.view<NoPassPlatform, Position, Hitbox, Velocity>();
    auto oneWayPlatformView = registry.view<BottomPassPlatform, Position, Hitbox, Velocity>();

    for (auto&& [playerTag, playerPos, playerVel, playerHitbox, playerJump, prevPos] : playerView) {

        Position originalPos = { prevPos.x, prevPos.y };

        for (auto&& [p, platformPos, platformHitbox, platformVel] : platformView) {
            if (aabb_overlap_world(playerPos, playerHitbox, platformPos, platformHitbox)) {
                resolvePlatformCollision(playerPos, playerVel, playerHitbox, playerJump,
                    platformPos, platformHitbox, originalPos, platformVel);
                // Apply platform movement to player position directly
                playerVel.dx = platformVel.dx;
                
            }
        }

        for (auto&& [p, platformPos, platformHitbox, platformVel] : oneWayPlatformView) {
            if (aabb_overlap_world(playerPos, playerHitbox, platformPos, platformHitbox)) {
                resolveOneWayPlatformCollision(playerPos, playerVel, playerHitbox, playerJump,
                    platformPos, platformHitbox, originalPos, platformVel);
            }
        }
    }
}

void resolvePlatformCollision(Position& playerPos, Velocity& playerVel, const Hitbox& playerHitbox,
    Jump& playerJump, const Position& platformPos, const Hitbox& platformHitbox,
    const Position& originalPos, const Velocity& platformVel)
{
    float playerLeft = playerPos.x + playerHitbox.offset_x;
    float playerRight = playerLeft + playerHitbox.width;
    float playerTop = playerPos.y + playerHitbox.offset_y;
    float playerBottom = playerTop + playerHitbox.height;

    float platformLeft = platformPos.x + platformHitbox.offset_x;
    float platformRight = platformLeft + platformHitbox.width;
    float platformTop = platformPos.y + platformHitbox.offset_y;
    float platformBottom = platformTop + platformHitbox.height;

    float overlapLeft = playerRight - platformLeft;
    float overlapRight = platformRight - playerLeft;
    float overlapTop = playerBottom - platformTop;
    float overlapBottom = platformBottom - playerTop;

    // plus l'overlap est petit, plus ça veut dire que ça "vient" de toucher
    float minOverlapX = std::min(overlapLeft, overlapRight);
    float minOverlapY = std::min(overlapTop, overlapBottom);

    if (minOverlapX < minOverlapY) {
        if (overlapLeft < overlapRight) {
            playerPos.x = platformLeft - (playerHitbox.offset_x + playerHitbox.width);
        } else {
            playerPos.x = platformRight - playerHitbox.offset_x;
        }
        playerVel.dx = platformVel.dx;
        std::cout << "Horizontal collision resolved!" << std::endl;
    } else {
        if (overlapTop < overlapBottom) {
            playerPos.y = platformTop - (playerHitbox.offset_y + playerHitbox.height);
            if (playerVel.dy >= 0) {
                playerVel.dy = 0.0f;
                playerJump.isJumping = false; // Player can jump again
                playerJump.canJump = true; // Reset jump ability
                // std::cout << "Player landed on platform at Y: " << playerPos.y << std::endl;
            }
        } else {
            playerPos.y = platformBottom - playerHitbox.offset_y;
            if (playerVel.dy < 0) {
                playerVel.dy = 0.0f;
                std::cout << "Player hit ceiling!" << std::endl;
            }
        }
    }
}

void resolveOneWayPlatformCollision(Position& playerPos, Velocity& playerVel, const Hitbox& playerHitbox,
    Jump& playerJump, const Position& platformPos, const Hitbox& platformHitbox,
    const Position& originalPos, const Velocity& platformVel)
{
    if (playerVel.dy <= 0) {
        return; // Player is not falling, ignore collision
    }

    // Check if player was above the platform in the previous frame
    float originalPlayerBottom = originalPos.y + playerHitbox.offset_y + playerHitbox.height;
    float platformTop = platformPos.y + platformHitbox.offset_y;

    if (originalPlayerBottom <= platformTop + 5.0f) {
        playerPos.y = platformTop - (playerHitbox.offset_y + playerHitbox.height);
        playerVel.dy = 0.0f;
        playerJump.isJumping = false;
        playerJump.canJump = true;
        // std::cout << "Player landed on one-way platform!" << std::endl;
    }
}

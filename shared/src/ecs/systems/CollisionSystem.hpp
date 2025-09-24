/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** CollisionSystem - Handle collisions between entities
*/

#pragma once
#include "../Registry.hpp"
#include "components/Hitbox.hpp"
#include "components/Jump.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"

void collisionSystem(Registry& registry, float deltaTime);

void resolvePlatformCollision(Position& playerPos, Velocity& playerVel, const Hitbox& playerHitbox,
    Jump& playerJump, const Position& platformPos, const Hitbox& platformHitbox,
    const Position& originalPos);

void resolveOneWayPlatformCollision(Position& playerPos, Velocity& playerVel, const Hitbox& playerHitbox,
    Jump& playerJump, const Position& platformPos, const Hitbox& platformHitbox,
    const Position& originalPos);
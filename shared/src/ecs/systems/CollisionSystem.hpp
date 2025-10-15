/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** CollisionSystem - Handle collisions between entities
*/

#pragma once
#include "../Registry.hpp"
#include "components/Hitbox.hpp"
#include "components/Position.hpp"
#include "components/RigidBody.hpp"
#include "components/Velocity.hpp"

/**
 * @brief Main collision detection and resolution system
 * @param registry The ECS registry containing entities to check for collisions
 * @param deltaTime Time elapsed since last update for physics calculations
 */
void collisionSystem(Registry& registry, float deltaTime);

/**
 * @brief Resolves collision between player and solid platform
 * @param playerPos Player's position component (modified if collision occurs)
 * @param playerVel Player's velocity component (modified if collision occurs)
 * @param playerHitbox Player's hitbox component
 * @param platformPos Platform's position component
 * @param platformHitbox Platform's hitbox component
 * @param originalPos Player's position before movement
 * @param platformVel Platform's velocity component
 * @param isPlayerOnPlatform Output flag indicating if player is standing on platform
 */
void resolvePlatformCollision(Position& playerPos, Velocity& playerVel, const Hitbox& playerHitbox,
    const Position& platformPos, const Hitbox& platformHitbox,
    const Position& originalPos, const Velocity& platformVel, RigidBody& rb);

/**
 * @brief Resolves collision between player and one-way platform (can jump through from below)
 * @param playerPos Player's position component (modified if collision occurs)
 * @param playerVel Player's velocity component (modified if collision occurs)
 * @param playerHitbox Player's hitbox component
 * @param platformPos Platform's position component
 * @param platformHitbox Platform's hitbox component
 * @param originalPos Player's position before movement
 * @param platformVel Platform's velocity component
 * @param isPlayerOnPlatform Output flag indicating if player is standing on platform
 */
void resolveOneWayPlatformCollision(Position& playerPos, Velocity& playerVel, const Hitbox& playerHitbox,
    const Position& platformPos, const Hitbox& platformHitbox,
    const Position& originalPos, const Velocity& platformVel, RigidBody& rb);

/**
 * @brief Checks if an entity is standing on a platform even without collision overlap
 * @param pos Entity's position component
 * @param hitbox Entity's hitbox component
 * @param rb Entity's rigid body component (modified to set isOnGround)
 * @param platformView View of regular platforms
 * @param oneWayPlatformView View of one-way platforms
 */
void checkGroundContact(const Position& pos, const Hitbox& hitbox, RigidBody& rb,
    const auto& platformView, const auto& oneWayPlatformView);

/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SpriteManager - Manages sprite creation and attachment to entities
*/

#pragma once

#include "Entity.hpp"
#include "Registry.hpp"

namespace SpriteManager {
/**
 * @brief Adds a sprite component to a player entity, automatically sizing and centering it
 * @param registry The ECS registry
 * @param entity The entity to add the sprite to
 * @param posX Entity X position (unused, for future compatibility)
 * @param posY Entity Y position (unused, for future compatibility)
 */
void addPlayerSprite(Registry& registry, Entity entity, float posX, float posY, float sizeFactor = 1.0f);

/**
 * @brief Adds an animated sprite component to a projectile entity, automatically sizing and centering it
 * @param registry The ECS registry
 * @param entity The entity to add the sprite to
 * @param posX Entity X position (unused, for future compatibility)
 * @param posY Entity Y position (unused, for future compatibility)
 */
void addProjectileSprite(Registry& registry, Entity entity, float posX, float posY, float sizeFactor = 1.0f);

/**
 * @brief Adds a static sprite component to a platform entity, automatically sizing and centering it
 * @param registry The ECS registry
 * @param entity The entity to add the sprite to
 * @param posX Entity X position (unused, for future compatibility)
 * @param posY Entity Y position (unused, for future compatibility)
 */
void addPlatformSprite(Registry& registry, Entity entity, float posX, float posY, float sizeFactor = 1.0f);

/**
 * @brief Adds an animated sprite component to an enemy entity, automatically sizing and centering it
 * @param registry The ECS registry
 * @param entity The entity to add the sprite to
 * @param posX Entity X position (unused, for future compatibility)
 * @param posY Entity Y position (unused, for future compatibility)
 */
void addEnemySprite(Registry& registry, Entity entity, float posX, float posY, float sizeFactor = 1.0f);
} // namespace SpriteManager

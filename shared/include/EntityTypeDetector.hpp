/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Entity Type Detector - Automatic entity type detection from ECS tags
*/

#pragma once

#include "EntityType.hpp"
#include "ecs/Registry.hpp"

/**
 * @brief Automatically detects EntityType from ECS component tags
 * Replaces hardcoded tag checking with a clean, extensible system
 */
class EntityTypeDetector {
public:
    /**
     * @brief Detect entity type from its ECS tags
     * @param registry The ECS registry
     * @param entity The entity to check
     * @return The detected EntityType, or throws if no type can be determined
     */
    static EntityType detectEntityType(const Registry& registry, Entity entity);

private:
    // Tag checking helpers
    static bool isPlayer(const Registry& registry, Entity entity);
    static bool isProjectile(const Registry& registry, Entity entity);
    static bool isPlatform(const Registry& registry, Entity entity);
    static bool isEnemy(const Registry& registry, Entity entity);
    static bool isBoss(const Registry& registry, Entity entity);
    static bool isPowerUp(const Registry& registry, Entity entity);
};

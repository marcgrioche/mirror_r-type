/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Entity Type Definitions
*/

#pragma once

#include <string>

/**
 * @brief Enumeration of all entity types in the game
 * Used for serialization, deserialization, and factory registration
 */
enum class EntityType {
    PLAYER = 0,
    PROJECTILE = 1,
    PLATFORM = 2,
    ENEMY = 3,
    BOSS = 4,
    POWERUP = 5,
    // Add new entity types here - serialization is automatic
};

/**
 * @brief Convert EntityType to string for debugging/logging
 */
inline const char* entityTypeToString(EntityType type)
{
    switch (type) {
    case EntityType::PLAYER:
        return "PLAYER";
    case EntityType::PROJECTILE:
        return "PROJECTILE";
    case EntityType::PLATFORM:
        return "PLATFORM";
    case EntityType::ENEMY:
        return "ENEMY";
    case EntityType::BOSS:
        return "BOSS";
    case EntityType::POWERUP:
        return "POWERUP";
    default:
        return "UNKNOWN";
    }
}

/**
 * @brief Convert string to EntityType
 * @return EntityType or throws std::invalid_argument if invalid
 */
EntityType stringToEntityType(const std::string& str);

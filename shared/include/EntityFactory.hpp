/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Entity Factory System
*/

#pragma once

#include "EntityData.hpp"
#include "EntityType.hpp"
#include "ecs/Registry.hpp"
#include <functional>
#include <unordered_map>

/**
 * @brief Central factory for creating entities
 * Replaces hardcoded entity creation with a clean, extensible system
 */
class EntityFactory {
public:
    /**
     * @brief Get the singleton instance
     */
    static EntityFactory& getInstance();

    /**
     * @brief Register a factory function for an entity type
     */
    void registerFactory(EntityType type, std::function<Entity(Registry&, const EntityData&)> factory);

    /**
     * @brief Create an entity using the registered factory
     * @throws std::runtime_error if type not registered or validation fails
     */
    Entity create(Registry& registry, const EntityData& data);

    /**
     * @brief Check if a factory is registered for the given type
     */
    bool isRegistered(EntityType type) const;

    /**
     * @brief Create EntityData builder for a specific type
     */
    EntityDataBuilder createDataBuilder(EntityType type);

private:
    EntityFactory() = default;
    std::unordered_map<EntityType, std::function<Entity(Registry&, const EntityData&)>> factories_;
};

/**
 * @brief Initialize all entity factories
 * Call this once at startup after registering all entity types
 */
void initializeEntityFactories();

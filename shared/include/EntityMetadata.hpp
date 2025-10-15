/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Entity Metadata System
*/

#pragma once

#include "EntityType.hpp"
#include "ecs/Registry.hpp"
#include <functional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

/**
 * @brief Types of properties that entities can have
 */
enum class PropertyType {
    INT,
    UINT32,
    FLOAT,
    DOUBLE,
    STRING,
    VEC2_FLOAT, // Position, Velocity
    VEC2_INT, // Hitbox offset
    VEC4_FLOAT, // Hitbox (width, height, offset_x, offset_y)
    BOOL,
    ENTITY_ID, // For Parent relationships
    POWERUP_TYPE // Special enum
};

/**
 * @brief Represents a single property of an entity
 */
struct EntityProperty {
    std::string name;
    PropertyType type;
    bool required;
    std::variant<int, uint32_t, float, double, std::string> defaultValue;

    EntityProperty(const std::string& n, PropertyType t, bool req = true)
        : name(n)
        , type(t)
        , required(req)
    {
    }

    EntityProperty(const std::string& n, PropertyType t, bool req,
        std::variant<int, uint32_t, float, double, std::string> def)
        : name(n)
        , type(t)
        , required(req)
        , defaultValue(def)
    {
    }
};

/**
 * @brief Forward declaration for EntityData
 */
struct EntityData;

/**
 * @brief Metadata for an entity type, including serialization info and factory
 */
struct EntityMetadata {
    EntityType type;
    std::string name;
    std::vector<EntityProperty> properties;
    std::function<Entity(Registry&, const EntityData&)> factory;

    /**
     * @brief Get property by name
     */
    const EntityProperty* getProperty(const std::string& name) const;

    /**
     * @brief Check if all required properties are present in data
     */
    bool validateProperties(const EntityData& data) const;
};

/**
 * @brief Registry of all entity metadata
 */
class EntityMetadataRegistry {
public:
    static EntityMetadataRegistry& getInstance();

    /**
     * @brief Register metadata for an entity type
     */
    void registerMetadata(const EntityMetadata& metadata);

    /**
     * @brief Get metadata for an entity type
     */
    const EntityMetadata* getMetadata(EntityType type) const;

    /**
     * @brief Get all registered metadata
     */
    const std::unordered_map<EntityType, EntityMetadata>& getAllMetadata() const;

private:
    EntityMetadataRegistry() = default;
    std::unordered_map<EntityType, EntityMetadata> metadata_;
};

/**
 * @brief Initialize all entity metadata
 * Call this once at startup to register all entity types
 */
void initializeEntityMetadata();

/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Component Mapper - Maps property names to ECS components
*/

#pragma once

#include "EntityData.hpp"
#include "ecs/Registry.hpp"
#include <functional>
#include <typeindex>
#include <unordered_map>

/**
 * @brief Maps property names to ECS component types and provides extraction/application functions
 * Enables automatic serialization/deserialization of entity properties
 */
class ComponentMapper {
public:
    using ExtractorFunc = std::function<PropertyValue(Entity, const Registry&)>;
    using ApplicatorFunc = std::function<void(Entity, Registry&, const PropertyValue&)>;

    /**
     * @brief Get the singleton instance
     */
    static ComponentMapper& getInstance();

    /**
     * @brief Register a mapping between a property name and component operations
     * @param propertyName The property name used in metadata
     * @param extractor Function to extract property value from entity
     * @param applicator Function to apply property value to entity
     */
    void registerMapping(const std::string& propertyName,
        ExtractorFunc extractor,
        ApplicatorFunc applicator);

    /**
     * @brief Extract a property value from an entity
     * @param entity The entity to extract from
     * @param registry The ECS registry
     * @param propertyName The property name
     * @return PropertyValue containing the extracted data
     * @throws std::runtime_error if property not registered or extraction fails
     */
    PropertyValue extractProperty(Entity entity, const Registry& registry, const std::string& propertyName) const;

    /**
     * @brief Apply a property value to an entity
     * @param entity The entity to apply to
     * @param registry The ECS registry
     * @param propertyName The property name
     * @param value The property value to apply
     * @throws std::runtime_error if property not registered or application fails
     */
    void applyProperty(Entity entity, Registry& registry, const std::string& propertyName, const PropertyValue& value) const;

    /**
     * @brief Check if a property mapping is registered
     */
    bool isRegistered(const std::string& propertyName) const;

private:
    ComponentMapper() = default;

    struct Mapping {
        ExtractorFunc extractor;
        ApplicatorFunc applicator;
    };

    std::unordered_map<std::string, Mapping> mappings_;
};

/**
 * @brief Initialize all component mappings
 * Call this once at startup to register all property-to-component mappings
 */
void initializeComponentMappings();

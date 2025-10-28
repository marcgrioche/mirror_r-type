/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Entity Serializer - Metadata-driven entity serialization
*/

#pragma once

#include "../src/ecs/Registry.hpp"
#include "../src/ecs/components/Damage.hpp"
#include "../src/ecs/components/Health.hpp"
#include "../src/ecs/components/Hitbox.hpp"
#include "../src/ecs/components/Lifetime.hpp"
#include "../src/ecs/components/Parent.hpp"
#include "../src/ecs/components/Position.hpp"
#include "../src/ecs/components/PowerUp.hpp"
#include "../src/ecs/components/Velocity.hpp"
#include "EntityData.hpp"
#include "EntityMetadata.hpp"
#include "EntityType.hpp"
#include "EntityTypeDetector.hpp"
#include "Message.hpp"

/**
 * @brief Handles automatic serialization and deserialization of entities using metadata
 * Replaces hardcoded property serialization with clean, automatic system
 */
class EntitySerializer {
public:
    /**
     * @brief Serialize an entity to a network message using its metadata
     * @param msg The message to write entity data to
     * @param entity The entity to serialize
     * @param registry The ECS registry containing the entity
     * @param usernames The players usernames map for special handling
     * @throws std::runtime_error if entity type cannot be determined or serialization fails
     */
    static void serializeEntity(Message& msg, Entity entity, const Registry& registry,
        const std::unordered_map<uint32_t, std::string>& usernames);

    /**
     * @brief Deserialize entity data from a network message
     * @param msg The message to read from
     * @param type The expected entity type
     * @return EntityData containing all deserialized properties
     * @throws std::runtime_error if deserialization fails or validation fails
     */
    static EntityData deserializeEntityData(Message& msg, EntityType type);

private:
    /**
     * @brief Extract property value from entity component
     * @param entity The entity to extract from
     * @param registry The ECS registry
     * @param property The property metadata
     * @return PropertyValue containing the extracted data
     */
    static PropertyValue extractPropertyValue(Entity entity, const Registry& registry, const EntityProperty& property);

    /**
     * @brief Apply property value to entity data during deserialization
     * @param data The EntityData to populate
     * @param property The property metadata
     * @param value The property value to apply
     */
    static void applyPropertyValue(EntityData& data, const EntityProperty& property, const PropertyValue& value);
};

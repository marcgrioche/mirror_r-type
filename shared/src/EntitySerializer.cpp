/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Entity Serializer Implementation
*/

#include "../include/EntitySerializer.hpp"
#include "../include/ComponentMapper.hpp"
#include "../include/PropertySerializer.hpp"
#include <stdexcept>

void EntitySerializer::serializeEntity(Message& msg, Entity entity, const Registry& registry,
    const std::unordered_map<uint32_t, std::string>& usernames)
{
    EntityType type = EntityTypeDetector::detectEntityType(registry, entity);

    const EntityMetadata* metadata = EntityMetadataRegistry::getInstance().getMetadata(type);
    if (!metadata) {
        throw std::runtime_error("No metadata registered for entity type: " + std::string(entityTypeToString(type)));
    }

    for (const auto& property : metadata->properties) {
        try {
            PropertyValue value = ComponentMapper::getInstance().extractProperty(entity, registry, property.name);
            if (std::holds_alternative<std::string>(value)) {
                if (std::get<std::string>(value).empty()) {
                    auto itUsername = usernames.find(entity.id);
                    if (itUsername != usernames.end()) {
                        std::cout << "Property serialized: " << property.name << ", value: " << itUsername->second << std::endl;
                        msg.write(itUsername->second);
                    } else {
                        msg.write("");
                    }
                }
            }
            PropertySerializer::serializeProperty(msg, value, property.type);
        } catch (const std::exception& e) {
            throw std::runtime_error("Failed to serialize property '" + property.name + "' for entity type " + std::string(entityTypeToString(type)) + ": " + e.what());
        }
    }
}

EntityData EntitySerializer::deserializeEntityData(Message& msg, EntityType type)
{
    EntityData data(type);

    const EntityMetadata* metadata = EntityMetadataRegistry::getInstance().getMetadata(type);
    if (!metadata) {
        throw std::runtime_error("No metadata registered for entity type: " + std::string(entityTypeToString(type)));
    }

    for (const auto& property : metadata->properties) {
        try {
            PropertyValue value = PropertySerializer::deserializeProperty(msg, property.type);
            data.set(property.name, value);
        } catch (const std::exception& e) {
            throw std::runtime_error("Failed to deserialize property '" + property.name + "' for entity type " + std::string(entityTypeToString(type)) + ": " + e.what());
        }
    }

    return data;
}

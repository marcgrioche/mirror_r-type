/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Entity Metadata Implementation
*/

#include "../include/EntityMetadata.hpp"
#include "../include/EntityData.hpp"
#include <algorithm>

const EntityProperty* EntityMetadata::getProperty(const std::string& name) const
{
    auto it = std::find_if(properties.begin(), properties.end(),
        [&name](const EntityProperty& prop) { return prop.name == name; });
    return it != properties.end() ? &(*it) : nullptr;
}

bool EntityMetadata::validateProperties(const EntityData& data) const
{
    for (const auto& prop : properties) {
        if (prop.required && !data.has(prop.name)) {
            return false;
        }
    }
    return true;
}

EntityMetadataRegistry& EntityMetadataRegistry::getInstance()
{
    static EntityMetadataRegistry instance;
    return instance;
}

void EntityMetadataRegistry::registerMetadata(const EntityMetadata& metadata)
{
    metadata_[metadata.type] = metadata;
}

const EntityMetadata* EntityMetadataRegistry::getMetadata(EntityType type) const
{
    auto it = metadata_.find(type);
    return it != metadata_.end() ? &it->second : nullptr;
}

const std::unordered_map<EntityType, EntityMetadata>& EntityMetadataRegistry::getAllMetadata() const
{
    return metadata_;
}

void initializeEntityMetadata()
{
    // This will be called at startup to register all entity types
    // Implementation will be added when we create the actual factory functions
}

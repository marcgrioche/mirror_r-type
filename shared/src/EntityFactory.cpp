/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Entity Factory Implementation
*/

#include "../include/EntityFactory.hpp"
#include "../include/EntityMetadata.hpp"
#include <stdexcept>

EntityFactory& EntityFactory::getInstance()
{
    static EntityFactory instance;
    return instance;
}

void EntityFactory::registerFactory(EntityType type, std::function<Entity(Registry&, const EntityData&)> factory)
{
    factories_[type] = std::move(factory);
}

Entity EntityFactory::create(Registry& registry, const EntityData& data)
{
    auto it = factories_.find(data.getType());
    if (it == factories_.end()) {
        throw std::runtime_error("No factory registered for entity type: " + std::string(entityTypeToString(data.getType())));
    }

    const EntityMetadata* metadata = EntityMetadataRegistry::getInstance().getMetadata(data.getType());
    if (metadata && !metadata->validateProperties(data)) {
        throw std::runtime_error("Entity data validation failed for type: " + std::string(entityTypeToString(data.getType())));
    }

    return it->second(registry, data);
}

bool EntityFactory::isRegistered(EntityType type) const
{
    return factories_.find(type) != factories_.end();
}

EntityDataBuilder EntityFactory::createDataBuilder(EntityType type)
{
    return EntityDataBuilder(type);
}

void initializeEntityFactories()
{
    // This will be implemented when we migrate the existing entity creation functions
    // to use the new factory system
}

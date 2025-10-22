/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Component Mapper Implementation
*/

#include "../include/ComponentMapper.hpp"
#include "../src/ecs/components/Damage.hpp"
#include "../src/ecs/components/Health.hpp"
#include "../src/ecs/components/Hitbox.hpp"
#include "../src/ecs/components/Lifetime.hpp"
#include "../src/ecs/components/Parent.hpp"
#include "../src/ecs/components/Position.hpp"
#include "../src/ecs/components/PowerUp.hpp"
#include "../src/ecs/components/Tags.hpp"
#include "../src/ecs/components/Velocity.hpp"
#include <stdexcept>

ComponentMapper& ComponentMapper::getInstance()
{
    static ComponentMapper instance;
    return instance;
}

void ComponentMapper::registerMapping(const std::string& propertyName,
    ExtractorFunc extractor,
    ApplicatorFunc applicator)
{
    mappings_[propertyName] = Mapping { extractor, applicator };
}

PropertyValue ComponentMapper::extractProperty(Entity entity, const Registry& registry, const std::string& propertyName) const
{
    auto it = mappings_.find(propertyName);
    if (it == mappings_.end()) {
        throw std::runtime_error("No mapping registered for property: " + propertyName);
    }

    try {
        return it->second.extractor(entity, registry);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to extract property '" + propertyName + "': " + e.what());
    }
}

void ComponentMapper::applyProperty(Entity entity, Registry& registry, const std::string& propertyName, const PropertyValue& value) const
{
    auto it = mappings_.find(propertyName);
    if (it == mappings_.end()) {
        throw std::runtime_error("No mapping registered for property: " + propertyName);
    }

    try {
        it->second.applicator(entity, registry, value);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to apply property '" + propertyName + "': " + e.what());
    }
}

bool ComponentMapper::isRegistered(const std::string& propertyName) const
{
    return mappings_.find(propertyName) != mappings_.end();
}

void initializeComponentMappings()
{
    ComponentMapper& mapper = ComponentMapper::getInstance();

    // Position component mapping
    mapper.registerMapping(
        "position",
        [](Entity entity, const Registry& registry) -> PropertyValue {
            if (!registry.has<Position>(entity)) {
                throw std::runtime_error("Entity missing Position component");
            }
            const auto& pos = registry.get<Position>(entity);
            return Vector2 { pos.v.x, pos.v.y };
        },
        [](Entity entity, Registry& registry, const PropertyValue& value) {
            const auto& pos = std::get<Vector2>(value);
            if (registry.has<Position>(entity)) {
                auto& position = registry.get<Position>(entity);
                position.v.x = pos.x;
                position.v.y = pos.y;
            } else {
                registry.add<Position>(entity, Position { pos.x, pos.y });
            }
        });

    // Velocity component mapping
    mapper.registerMapping(
        "velocity",
        [](Entity entity, const Registry& registry) -> PropertyValue {
            if (!registry.has<Velocity>(entity)) {
                throw std::runtime_error("Entity missing Velocity component");
            }
            const auto& vel = registry.get<Velocity>(entity);
            return Vector2 { vel.v.x, vel.v.y };
        },
        [](Entity entity, Registry& registry, const PropertyValue& value) {
            const auto& vel = std::get<Vector2>(value);
            if (registry.has<Velocity>(entity)) {
                auto& velocity = registry.get<Velocity>(entity);
                velocity.v.x = vel.x;
                velocity.v.y = vel.y;
            } else {
                registry.add<Velocity>(entity, Velocity { vel.x, vel.y });
            }
        });

    // Health component mapping
    mapper.registerMapping(
        "health",
        [](Entity entity, const Registry& registry) -> PropertyValue {
            if (!registry.has<Health>(entity)) {
                throw std::runtime_error("Entity missing Health component");
            }
            const auto& health = registry.get<Health>(entity);
            return health.hp;
        },
        [](Entity entity, Registry& registry, const PropertyValue& value) {
            int hp = std::get<int>(value);
            if (registry.has<Health>(entity)) {
                auto& health = registry.get<Health>(entity);
                health.hp = hp;
            } else {
                registry.add<Health>(entity, Health { hp });
            }
        });

    // Damage component mapping
    mapper.registerMapping(
        "damage",
        [](Entity entity, const Registry& registry) -> PropertyValue {
            if (!registry.has<Damage>(entity)) {
                throw std::runtime_error("Entity missing Damage component");
            }
            const auto& damage = registry.get<Damage>(entity);
            return damage.value;
        },
        [](Entity entity, Registry& registry, const PropertyValue& value) {
            float dmg = std::get<float>(value);
            if (registry.has<Damage>(entity)) {
                auto& damage = registry.get<Damage>(entity);
                damage.value = dmg;
            } else {
                registry.add<Damage>(entity, Damage { dmg });
            }
        });

    // Hitbox component mapping
    mapper.registerMapping(
        "hitbox",
        [](Entity entity, const Registry& registry) -> PropertyValue {
            if (!registry.has<Hitbox>(entity)) {
                throw std::runtime_error("Entity missing Hitbox component");
            }
            const auto& hitbox = registry.get<Hitbox>(entity);
            return std::array<float, 4> { hitbox.width, hitbox.height, hitbox.offset_x, hitbox.offset_y };
        },
        [](Entity entity, Registry& registry, const PropertyValue& value) {
            const auto& hb = std::get<std::array<float, 4>>(value);
            if (registry.has<Hitbox>(entity)) {
                auto& hitbox = registry.get<Hitbox>(entity);
                hitbox.width = hb[0];
                hitbox.height = hb[1];
                hitbox.offset_x = hb[2];
                hitbox.offset_y = hb[3];
            } else {
                registry.add<Hitbox>(entity, Hitbox { hb[0], hb[1], hb[2], hb[3] });
            }
        });

    // Lifetime component mapping
    mapper.registerMapping(
        "lifetime",
        [](Entity entity, const Registry& registry) -> PropertyValue {
            if (!registry.has<Lifetime>(entity)) {
                throw std::runtime_error("Entity missing Lifetime component");
            }
            const auto& lifetime = registry.get<Lifetime>(entity);
            return lifetime.value;
        },
        [](Entity entity, Registry& registry, const PropertyValue& value) {
            float time = std::get<float>(value);
            if (registry.has<Lifetime>(entity)) {
                auto& lifetime = registry.get<Lifetime>(entity);
                lifetime.value = time;
            } else {
                registry.add<Lifetime>(entity, Lifetime { time });
            }
        });

    // Power-up type mapping
    mapper.registerMapping(
        "powerup_type",
        [](Entity entity, const Registry& registry) -> PropertyValue {
            if (!registry.has<PowerUp>(entity)) {
                throw std::runtime_error("Entity missing PowerUp component");
            }
            const auto& powerup = registry.get<PowerUp>(entity);
            return powerup.type;
        },
        [](Entity entity, Registry& registry, const PropertyValue& value) {
            PowerUpType type = std::get<PowerUpType>(value);
            if (registry.has<PowerUp>(entity)) {
                auto& powerup = registry.get<PowerUp>(entity);
                powerup.type = type;
            } else {
                // Create PowerUp component with default values
                registry.add<PowerUp>(entity, PowerUp { true, type, 0.0f, 0.0f });
            }
        });

    // Power-up effect duration mapping
    mapper.registerMapping(
        "effect_duration",
        [](Entity entity, const Registry& registry) -> PropertyValue {
            if (!registry.has<PowerUp>(entity)) {
                throw std::runtime_error("Entity missing PowerUp component");
            }
            const auto& powerup = registry.get<PowerUp>(entity);
            return powerup.effect_duration;
        },
        [](Entity entity, Registry& registry, const PropertyValue& value) {
            float duration = std::get<float>(value);
            if (registry.has<PowerUp>(entity)) {
                auto& powerup = registry.get<PowerUp>(entity);
                powerup.effect_duration = duration;
            } else {
                // Create PowerUp component with default values
                registry.add<PowerUp>(entity, PowerUp { true, PowerUpType::HEAL, duration, 0.0f });
            }
        });

    // Parent entity mapping
    mapper.registerMapping(
        "parent",
        [](Entity entity, const Registry& registry) -> PropertyValue {
            if (!registry.has<Parent>(entity)) {
                throw std::runtime_error("Entity missing Parent component");
            }
            const auto& parent = registry.get<Parent>(entity);
            return parent.parent;
        },
        [](Entity entity, Registry& registry, const PropertyValue& value) {
            Entity parentEntity = std::get<Entity>(value);
            if (registry.has<Parent>(entity)) {
                auto& parent = registry.get<Parent>(entity);
                parent.parent = parentEntity;
            } else {
                registry.add<Parent>(entity, Parent { parentEntity });
            }
        });

    // Player ID mapping (special case - stored in usernames map, not a component)
    mapper.registerMapping(
        "player_id",
        [](Entity entity, const Registry& registry) -> PropertyValue {
            (void)entity;
            (void)registry;
            // This would need access to the playerEntities map
            // For now, return a default - this will be handled specially
            return static_cast<uint32_t>(0);
        },
        [](Entity entity, Registry& registry, const PropertyValue& value) {
            (void)entity;
            (void)registry;
            (void)value;
            // This will be handled by the deserialization context
            // The player ID is used to look up the entity, not stored as a component
        });

    // Username mapping (special case - stored in usernames map, not a component)
    mapper.registerMapping(
        "username",
        [](Entity entity, const Registry& registry) -> PropertyValue {
            (void)entity;
            (void)registry;
            // This would need access to the usernames map
            // For now, return empty string - this will be handled specially
            return std::string("");
        },
        [](Entity entity, Registry& registry, const PropertyValue& value) {
            (void)entity;
            (void)registry;
            (void)value;
            // Usernames are handled by the deserialization context
            // Not stored as components
        });
}

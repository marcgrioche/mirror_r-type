#include "../../include/game_instance/GameInstanceSerialization.hpp"
#include "../ecs/components/Position.hpp"
#include "../ecs/components/Velocity.hpp"
#include "../ecs/components/Health.hpp"
#include "../ecs/components/Hitbox.hpp"
#include "../ecs/components/Damage.hpp"
#include "../ecs/components/Lifetime.hpp"
#include "../ecs/components/PowerUp.hpp"
#include "../ecs/components/Tags.hpp"
#include "../ecs/components/Parent.hpp"

std::vector<uint8_t> GameInstanceSerialization::serializeGameState(
    const Registry& registry,
    uint32_t currentTick,
    const std::unordered_map<uint32_t, Entity>& playerEntities)
{
    Message msg(MessageType::GAME_STATE);
    msg.write(static_cast<uint32_t>(currentTick));
    msg.write(static_cast<uint8_t>(playerEntities.size()));

    for (const auto& [playerId, entity] : playerEntities) {
        msg.write(static_cast<uint32_t>(entity.id));

        if (registry.has<Position>(entity)) {
            const auto& pos = registry.get<Position>(entity);
            msg.write(pos.v.x);
            msg.write(pos.v.y);
        } else {
            msg.write(0.0f);
            msg.write(0.0f);
        }

        if (registry.has<Health>(entity)) {
            const auto& health = registry.get<Health>(entity);
            msg.write(static_cast<uint32_t>(health.hp));
        } else {
            msg.write(static_cast<uint32_t>(100));
        }
    }

    return msg.getPayload();
}

void GameInstanceSerialization::deserializeGameState(const std::vector<uint8_t>& data)
{
    (void)data; // TODO: Implement state deserialization for rollback
}

Message GameInstanceSerialization::serializeEntitySpawn(
    const Registry& registry,
    Entity entity,
    const std::unordered_map<uint32_t, Entity>& playerEntities,
    const std::unordered_map<uint32_t, std::string>& usernames)
{
    std::vector<Entity> entities = { entity };
    return serializeEntityBatch(registry, entities, playerEntities, usernames);
}

Message GameInstanceSerialization::serializeEntityBatch(
    const Registry& registry,
    const std::vector<Entity>& entities,
    const std::unordered_map<uint32_t, Entity>& playerEntities,
    const std::unordered_map<uint32_t, std::string>& usernames)
{
    Message msg(MessageType::SPAWN_ENTITY);
    msg.write(static_cast<uint8_t>(entities.size()));

    for (Entity entity : entities) {
        uint8_t entityType = 255;

        if (registry.has<PlayerTag>(entity)) {
            entityType = 0; // Player
        } else if (registry.has<ProjectileTag>(entity)) {
            entityType = 1; // Projectile
        } else if (registry.has<PlatformTag>(entity)) {
            entityType = 2; // Platform
        } else if (registry.has<EnemyTag>(entity)) {
            entityType = 3; // Enemy
        } else if (registry.has<PowerUpTag>(entity)) {
            entityType = 4; // Power-up
        }

        if (entityType == 255) {
            continue; // Skip invalid entities
        }

        msg.write(static_cast<uint32_t>(entity.id));
        msg.write(entityType);

        if (registry.has<Position>(entity)) {
            auto& pos = registry.get<Position>(entity);
            msg.write(pos.v.x);
            msg.write(pos.v.y);
        } else {
            continue; // Skip entities without position
        }

        if (entityType == 0) { // Player
            if (registry.has<Health>(entity)) {
                auto& health = registry.get<Health>(entity);
                msg.write(static_cast<uint32_t>(health.hp));
            }

            if (registry.has<Hitbox>(entity)) {
                auto& hitbox = registry.get<Hitbox>(entity);
                msg.write(hitbox.width);
                msg.write(hitbox.height);
                msg.write(hitbox.offset_x);
                msg.write(hitbox.offset_y);
            }

            const auto playerId = findPlayerIdByEntity(entity, playerEntities);
            msg.write(playerId);
            auto itUsername = usernames.find(entity.id);
            if (itUsername != usernames.end()) {
                msg.write(itUsername->second);
            } else {
                msg.write(std::to_string(playerId));
            }

        } else if (entityType == 1) { // Projectile
            if (registry.has<Velocity>(entity)) {
                auto& vel = registry.get<Velocity>(entity);
                msg.write(vel.v.x);
                msg.write(vel.v.y);
            }

            if (registry.has<Damage>(entity)) {
                auto& damage = registry.get<Damage>(entity);
                msg.write(damage.value);
            }

            if (registry.has<Hitbox>(entity)) {
                auto& hitbox = registry.get<Hitbox>(entity);
                msg.write(hitbox.width);
                msg.write(hitbox.height);
                msg.write(hitbox.offset_x);
                msg.write(hitbox.offset_y);
            }

            if (registry.has<Parent>(entity)) {
                auto& parent = registry.get<Parent>(entity);
                msg.write(static_cast<uint32_t>(parent.parent.id));
                msg.write(static_cast<uint32_t>(parent.parent.version));
            } else {
                msg.write(static_cast<uint32_t>(0));
                msg.write(static_cast<uint32_t>(0));
            }

            if (registry.has<Lifetime>(entity)) {
                auto& lifetime = registry.get<Lifetime>(entity);
                msg.write(lifetime.value);
            }

        } else if (entityType == 2) { // Platform
            if (registry.has<Hitbox>(entity)) {
                auto& hitbox = registry.get<Hitbox>(entity);
                msg.write(hitbox.width);
                msg.write(hitbox.height);
                msg.write(hitbox.offset_x);
                msg.write(hitbox.offset_y);
            }

        } else if (entityType == 3) { // Enemy
            if (registry.has<Velocity>(entity)) {
                auto& vel = registry.get<Velocity>(entity);
                msg.write(vel.v.x);
                msg.write(vel.v.y);
            }

            if (registry.has<Health>(entity)) {
                auto& health = registry.get<Health>(entity);
                msg.write(static_cast<uint32_t>(health.hp));
            }

            if (registry.has<Hitbox>(entity)) {
                auto& hitbox = registry.get<Hitbox>(entity);
                msg.write(hitbox.width);
                msg.write(hitbox.height);
                msg.write(hitbox.offset_x);
                msg.write(hitbox.offset_y);
            }
        } else if (entityType == 4) { // Power-up
            if (registry.has<PowerUp>(entity)) {
                auto& powerUp = registry.get<PowerUp>(entity);
                msg.write(static_cast<uint8_t>(powerUp.type));
                msg.write(powerUp.effect_duration);
            }

            if (registry.has<Hitbox>(entity)) {
                auto& hitbox = registry.get<Hitbox>(entity);
                msg.write(hitbox.width);
                msg.write(hitbox.height);
                msg.write(hitbox.offset_x);
                msg.write(hitbox.offset_y);
            }

            if (registry.has<Lifetime>(entity)) {
                auto& lifetime = registry.get<Lifetime>(entity);
                msg.write(lifetime.value);
            }
        }
    }

    return msg;
}

uint32_t GameInstanceSerialization::findPlayerIdByEntity(
    const Entity& entity,
    const std::unordered_map<uint32_t, Entity>& playerEntities)
{
    for (const auto& [playerId, playerEntity] : playerEntities) {
        if (playerEntity == entity) {
            return playerId;
        }
    }
    return 0;
}

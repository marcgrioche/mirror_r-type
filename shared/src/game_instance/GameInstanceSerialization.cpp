#include "../../include/game_instance/GameInstanceSerialization.hpp"
#include "../../include/EntitySerializer.hpp"
#include "../../include/EntityTypeDetector.hpp"
#include "../../include/PlayerMovementState.hpp"
#include "../ecs/components/Damage.hpp"
#include "../ecs/components/Dash.hpp"
#include "../ecs/components/Health.hpp"
#include "../ecs/components/Hitbox.hpp"
#include "../ecs/components/Lifetime.hpp"
#include "../ecs/components/Parent.hpp"
#include "../ecs/components/PlayerInputState.hpp"
#include "../ecs/components/Position.hpp"
#include "../ecs/components/PowerUp.hpp"
#include "../ecs/components/RigidBody.hpp"
#include "../ecs/components/ServerEntityId.hpp"
#include "../ecs/components/Tags.hpp"
#include "../ecs/components/Velocity.hpp"

PlayerMovementState GameInstanceSerialization::calculatePlayerMovementState(const Registry& registry, Entity entity)
{
    if (registry.has<Dash>(entity) && registry.get<Dash>(entity).isDashing) {
        return PlayerMovementState::DASHING;
    }

    if (registry.has<RigidBody>(entity) && !registry.get<RigidBody>(entity).isOnGround) {
        return PlayerMovementState::JUMPING;
    }

    if (registry.has<PlayerInputState>(entity)) {
        const auto& inputState = registry.get<PlayerInputState>(entity);
        if (inputState.leftPressed || inputState.rightPressed) {
            return PlayerMovementState::RUNNING;
        }
    }

    return PlayerMovementState::IDLE;
}

FacingDirection GameInstanceSerialization::calculatePlayerFacingDirection(const Registry& registry, Entity entity)
{
    if (registry.has<PlayerInputState>(entity)) {
        const auto& inputState = registry.get<PlayerInputState>(entity);
        return inputState.lastFacingDirection;
    }

    if (registry.has<Velocity>(entity)) {
        const auto& velocity = registry.get<Velocity>(entity);
        if (velocity.v.x < 0)
            return FacingDirection::LEFT;
        if (velocity.v.x > 0)
            return FacingDirection::RIGHT;
    }

    return FacingDirection::RIGHT;
}

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

        PlayerMovementState movementState = calculatePlayerMovementState(registry, entity);
        FacingDirection facingDirection = calculatePlayerFacingDirection(registry, entity);
        msg.write(static_cast<uint8_t>(movementState));
        msg.write(static_cast<uint8_t>(facingDirection));
    }

    // Serialize boss entities
    const auto* bossStorage = registry.get_storage_if_exists<BossTag>();
    if (bossStorage) {
        const auto& entities = bossStorage->dense_entities_ref();
        msg.write(static_cast<uint8_t>(entities.size()));

        for (const auto& entity : entities) {
            const auto& health = registry.get<Health>(entity);
            msg.write(static_cast<uint32_t>(entity.id));
            msg.write(static_cast<uint32_t>(health.hp));
        }
    } else {
        msg.write(static_cast<uint8_t>(0));
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
        if (!registry.has<Position>(entity)) {
            continue;
        }

        EntityType type;
        try {
            type = EntityTypeDetector::detectEntityType(registry, entity);
        } catch (const std::exception&) {
            continue;
        }

        uint8_t entityType = static_cast<uint8_t>(type);

        msg.write(static_cast<uint32_t>(entity.id));
        msg.write(entityType);

        try {
            EntitySerializer::serializeEntity(msg, entity, registry);
        } catch (const std::exception&) {
            continue;
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

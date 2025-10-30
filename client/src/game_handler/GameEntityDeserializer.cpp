/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game Entity Deserializer - Handles entity creation from network messages
*/

#include "../../shared/include/EntityData.hpp"
#include "../../shared/include/EntityFactory.hpp"
#include "../../shared/include/EntityMetadataRegistration.hpp"
#include "../../shared/include/EntitySerializer.hpp"
#include "../../shared/include/EntityType.hpp"
#include "../../shared/include/PlayerMovementState.hpp"
#include "Game.hpp"
#include "ecs/components/Damage.hpp"
#include "ecs/components/Health.hpp"
#include "ecs/components/Hitbox.hpp"
#include "ecs/components/IsAttacking.hpp"
#include "ecs/components/Lifetime.hpp"
#include "ecs/components/PlayerSyncState.hpp"
#include "ecs/components/Position.hpp"
#include "ecs/components/PowerUp.hpp"
#include "ecs/components/ServerEntityId.hpp"
#include "ecs/components/Sprite.hpp"
#include "ecs/components/SpriteFactory.hpp"
#include "ecs/components/SpriteManager.hpp"
#include "ecs/components/Tags.hpp"
#include "ecs/components/Velocity.hpp"
#include <iostream>

void Game::deserializeAndCreateEntity(const Message& msg, Registry& registry)
{
    const_cast<Message&>(msg).resetReadPosition();

    uint8_t entityCount = msg.readU8();

    for (uint8_t i = 0; i < entityCount; ++i) {
        uint32_t entityId = msg.readU32();
        uint8_t entityTypeRaw = msg.readU8();
        EntityType type = static_cast<EntityType>(entityTypeRaw);

        EntityData data = EntitySerializer::deserializeEntityData(const_cast<Message&>(msg), type);

        Entity entity = EntityFactory::getInstance().create(registry, data);

        registry.add<ServerEntityId>(entity, ServerEntityId { entityId });

        attachSpriteToEntity(registry, entity, type, data);
    }
}

void Game::createPlayerFromMessage(const Message& msg, Registry& registry,
    uint32_t entityId, float posX, float posY)
{
    uint32_t healthValue = msg.readU32();
    float width = msg.readFloat();
    float height = msg.readFloat();
    float offsetX = msg.readFloat();
    float offsetY = msg.readFloat();
    uint32_t serverPlayerId = msg.readU32();
    uint8_t usernameLength = msg.readU8();

    // Extract the actual username
    std::string username = msg.readString(usernameLength);

    Entity entity = factories::createPlayer(
        registry,
        username,
        Position { posX, posY },
        Health { static_cast<int>(healthValue) },
        Hitbox { width, height, offsetX, offsetY },
        serverPlayerId);

    if (serverPlayerId == m_clientNetwork->getPlayerId()) {
        m_clientNetwork->resetTick();
    }

    // Entity entity2 = factories::createPlayer(registry,
    //     Position { posX, posY },
    //     Health { static_cast<int>(healthValue) },
    //     Hitbox { width, height, offsetX, offsetY },
    //     TextBox { username, 12 },
    //     100);

    registry.add<ServerEntityId>(entity, ServerEntityId { entityId });
    SpriteManager::addPlayerSprite(registry, entity, posX, posY, 1.5f);
    // SpriteManager::addPlayerSprite(registry, entity2, posX, posY, 1.5f);
}

void Game::createProjectileFromMessage(const Message& msg, Registry& registry,
    uint32_t entityId, float posX, float posY)
{
    float velX = msg.readFloat();
    float velY = msg.readFloat();
    float damageValue = msg.readFloat();
    float width = msg.readFloat();
    float height = msg.readFloat();
    float offsetX = msg.readFloat();
    float offsetY = msg.readFloat();
    uint32_t parentId = msg.readU32();
    uint32_t parentVersion = msg.readU32();
    float lifetimeValue = msg.readFloat();

    Entity projectile = factories::createProjectile(
        registry,
        Position { posX, posY },
        Velocity { velX, velY },
        Damage { damageValue },
        Hitbox { width, height, offsetX, offsetY },
        Parent { Entity { parentId, parentVersion } },
        Lifetime { lifetimeValue });

    registry.add<ServerEntityId>(projectile, ServerEntityId { entityId });
    SpriteManager::addProjectileSprite(registry, projectile, posX, posY, m_currentLevelData.getProjectileSizeFactor(), &m_currentLevelData);
}

void Game::createPlatformFromMessage(const Message& msg, Registry& registry,
    uint32_t entityId, float posX, float posY)
{
    float width = msg.readFloat();
    float height = msg.readFloat();
    float offsetX = msg.readFloat();
    float offsetY = msg.readFloat();

    Entity platform = factories::createOneWayPlatform(registry,
        posX,
        posY,
        0.0,
        0.0,
        width,
        height,
        offsetX,
        offsetY);

    registry.add<ServerEntityId>(platform, ServerEntityId { entityId });
    SpriteManager::addPlatformSprite(registry, platform, posX, posY);
}

void Game::createEnemyFromMessage(const Message& msg, Registry& registry,
    uint32_t entityId, float posX, float posY)
{
    float velX = msg.readFloat();
    float velY = msg.readFloat();
    uint32_t healthValue = msg.readU32();
    float width = msg.readFloat();
    float height = msg.readFloat();
    float offsetX = msg.readFloat();
    float offsetY = msg.readFloat();

    Entity enemy = factories::createEnemy(
        registry,
        Position { posX, posY },
        Health { static_cast<int>(healthValue) },
        Hitbox { width, height, offsetX, offsetY },
        Velocity { ENEMY_VELOCITY_X, ENEMY_VELOCITY_Y });

    if (registry.has<Velocity>(enemy)) {
        auto& velocity = registry.get<Velocity>(enemy);
        velocity.v.x = velX;
        velocity.v.y = velY;
    }

    registry.add<ServerEntityId>(enemy, ServerEntityId { entityId });
    // Attach enemy sprite based on level config
    SpriteManager::addEnemySprite(registry, enemy, posX, posY, m_currentLevelData.getEnemySizeFactor(), &m_currentLevelData);
}

void Game::createPowerUpFromMessage(const Message& msg, Registry& registry,
    uint32_t entityId, float posX, float posY)
{
    uint8_t powerUpTypeValue = msg.readU8();
    float effectDuration = msg.readFloat();
    float width = msg.readFloat();
    float height = msg.readFloat();
    float offsetX = msg.readFloat();
    float offsetY = msg.readFloat();
    float lifetimeValue = msg.readFloat();

    PowerUpType type = static_cast<PowerUpType>(powerUpTypeValue);

    Entity powerUp = factories::createPowerUp(
        registry,
        Position { posX, posY },
        Velocity { 0.0f, 0.0f }, // Stationary
        Hitbox { width, height, offsetX, offsetY },
        Lifetime { lifetimeValue },
        type,
        effectDuration);

    registry.add<ServerEntityId>(powerUp, ServerEntityId { entityId });
}

void Game::attachSpriteToEntity(Registry& registry, Entity entity, EntityType type, const EntityData& data)
{
    Vector2 position = data.get<Vector2>("position");

    switch (type) {
    case EntityType::PLAYER:
        SpriteManager::addPlayerSprite(registry, entity, position.x, position.y, 1.5f);
        break;
    case EntityType::PROJECTILE:
        SpriteManager::addProjectileSprite(registry, entity, position.x, position.y, m_currentLevelData.getProjectileSizeFactor(), &m_currentLevelData);
        break;
    case EntityType::PLATFORM:
        SpriteManager::addPlatformSprite(registry, entity, position.x, position.y, m_currentLevelData.getPlatformSizeFactor(), &m_currentLevelData);
        break;
    case EntityType::ENEMY:
        SpriteManager::addEnemySprite(registry, entity, position.x, position.y, m_currentLevelData.getEnemySizeFactor(), &m_currentLevelData);
        break;
    case EntityType::POWERUP:
        // TODO: PowerUp sprite attachment
        break;
    case EntityType::BOSS:
        SpriteManager::addBossSprite(registry, entity, position.x, position.y, m_currentLevelData.getBossSizeFactor(), &m_currentLevelData);
        break;
    default:
        std::cout << "Unknown entity type for sprite attachment: " << static_cast<int>(type) << std::endl;
        break;
    }
}

void Game::logUnknownEntityType(uint8_t entityType)
{
    std::cout << "Unknown entity type: " << static_cast<int>(entityType) << std::endl;
}

void Game::logEntityCreation(uint32_t entityId, uint8_t entityType, float posX, float posY)
{
    std::cout << "Created entity ID " << entityId << " of type " << static_cast<int>(entityType)
              << " at position (" << posX << ", " << posY << ")" << std::endl;
}

void Game::deserializeAndUpdateGameState(const Message& msg, Registry& registry)
{
    const_cast<Message&>(msg).resetReadPosition();

    uint32_t tick = msg.readU32();
    (void)tick;
    uint8_t numPlayers = msg.readU8();

    updateNonPredictedEntities(msg, registry, numPlayers, tick);

    // Update boss entities
    uint8_t numBosses = msg.readU8();
    for (uint8_t i = 0; i < numBosses; ++i) {
        uint32_t entityId = msg.readU32();
        uint32_t health = msg.readU32();
        uint8_t isAttacking = msg.readU8();
        uint8_t isHit = msg.readU8();
        float hitTime = msg.readFloat();

        Entity entity = findEntityByServerId(registry, entityId);
        if (entity.id != 0 && registry.has<Health>(entity)) {
            auto& healthComp = registry.get<Health>(entity);
            healthComp.hp = static_cast<int>(health);
            healthComp.hit = static_cast<int>(isHit);
            healthComp.hitTime = static_cast<float>(hitTime);
        }

        // Update the transient attack flag on the boss (used by SpriteAnimationSystem)
        if (entity.id != 0 && registry.has<BossTag>(entity) && registry.has<IsAttacking>(entity)) {
            auto& atk = registry.get<IsAttacking>(entity);
            atk.attacking = static_cast<int>(isAttacking);
        }

        // std::cout << "Boss " << entityId << " hit state: " << static_cast<int>(isHit) << std::endl;
    }
}

void Game::updateNonPredictedEntities(const Message& msg, Registry& registry,
    uint8_t numPlayers, uint32_t tick)
{
    for (uint8_t i = 0; i < numPlayers; ++i) {
        updateSingleEntity(msg, registry, tick);
    }
}

void Game::updateSingleEntity(const Message& msg, Registry& registry, uint32_t tick)
{
    uint32_t entityId = msg.readU32();
    float posX = msg.readFloat();
    float posY = msg.readFloat();
    uint32_t health = msg.readU32();
    uint8_t movementStateRaw = msg.readU8();
    uint8_t facingDirectionRaw = msg.readU8();

    PlayerMovementState movementState = static_cast<PlayerMovementState>(movementStateRaw);
    FacingDirection facingDirection = static_cast<FacingDirection>(facingDirectionRaw);

    Entity entity = findEntityByServerId(registry, entityId);
    if (entity.id == 0) {
        return;
    }

    updateEntityState(registry, entity, posX, posY, health, movementState, facingDirection, tick);
}

Entity Game::findEntityByServerId(Registry& registry, uint32_t serverId)
{
    auto view = registry.view<ServerEntityId>();
    for (auto it = view.begin(); it != view.end(); ++it) {
        Entity entity = it.entity();
        const auto& serverEntityId = registry.get<ServerEntityId>(entity);
        if (serverEntityId.id == serverId) {
            return entity;
        }
    }
    return Entity { 0, 0 };
}

Entity Game::findEntityByClientId(Registry& registry, const uint32_t clientId)
{
    auto view = registry.view<PlayerTag>();
    for (auto it = view.begin(); it != view.end(); ++it) {
        Entity entity = it.entity();
        const auto& playerTag = registry.get<PlayerTag>(entity);
        if (playerTag.playerId == clientId) {
            return entity;
        }
    }
    return Entity { 0, 0 };
}

void Game::updateEntityState(Registry& registry, Entity entity,
    float posX, float posY, uint32_t health, PlayerMovementState movementState,
    FacingDirection facingDirection, const uint32_t serverTick)
{
    updateEntityPosition(registry, entity, posX, posY, serverTick);
    updateEntityHealth(registry, entity, health);
    updateEntitySyncState(registry, entity, movementState, facingDirection);
}

void Game::updateEntityPosition(Registry& registry, Entity entity, float posX, float posY,
    const uint32_t serverTick)
{
    auto playerEntity = findEntityByClientId(registry, m_clientNetwork->getPlayerId());
    // auto playerTag = registry.get<PlayerTag>(entity);

    if (playerEntity == entity) {
        if (registry.has<Position>(playerEntity)) {
            auto& position = registry.get<Position>(playerEntity);
            // std::cout << "Server tick " << serverTick << " , POSX: " << posX << ", POSY: " << posY << std::endl;
            // std::cout << "client tick " << m_clientNetwork->getCurrentTick() << " , POSX: " << position.v.x << ", POSY: " << position.v.y << std::endl;
            position.v.x = posX;
            position.v.y = posY;
        }

        // Step 2: Reconciliation
        // Remove all confirmed inputs
        m_inputHistory.removeUpToTick(serverTick);

        // Step 3: Replay remaining inputs for prediction
        // for (const auto& frame : m_inputHistory.getHistory()) {
        //     auto actions = frame.actions;
        //     // Apply each frame's actions to the player as you do in your normal per-tick logic
        //     processLocalInputs(actions);
        // }

        // Optionally: update sprite position with interpolation (for smoothing)
        // updatePlayerSprite(registry, entity, posX, posY);
        return;
    }
    if (registry.has<Position>(entity)) {
        auto& position = registry.get<Position>(entity);
        position.v.x = posX;
        position.v.y = posY;
    }

    updatePlayerSprite(registry, entity, posX, posY);
}

void Game::updateEntityHealth(Registry& registry, Entity entity, uint32_t health)
{
    if (registry.has<Health>(entity)) {
        auto& healthComp = registry.get<Health>(entity);
        healthComp.hp = static_cast<int>(health);
    }
}

void Game::updateEntitySyncState(Registry& registry, Entity entity,
    PlayerMovementState movementState, FacingDirection facingDirection)
{
    if (!registry.has<PlayerSyncState>(entity)) {
        registry.add<PlayerSyncState>(entity, PlayerSyncState { movementState, facingDirection });
    } else {
        auto& syncState = registry.get<PlayerSyncState>(entity);
        syncState.movementState = movementState;
        syncState.facingDirection = facingDirection;
    }
}

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
#include "Game.hpp"
#include "ecs/components/Damage.hpp"
#include "ecs/components/Health.hpp"
#include "ecs/components/Hitbox.hpp"
#include "ecs/components/Lifetime.hpp"
#include "ecs/components/Position.hpp"
#include "ecs/components/PowerUp.hpp"
#include "ecs/components/ServerEntityId.hpp"
#include "ecs/components/Sprite.hpp"
#include "ecs/components/SpriteFactory.hpp"
#include "ecs/components/SpriteManager.hpp"
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
    (void)serverPlayerId;
    uint8_t usernameLength = msg.readU8();

    // Extract the actual username
    std::string username = msg.readString(usernameLength);

    Entity entity = factories::createPlayer(registry,
        Position { posX, posY },
        Health { static_cast<int>(healthValue) },
        Hitbox { width, height, offsetX, offsetY },
        TextBox { username, 12 });

    registry.add<ServerEntityId>(entity, ServerEntityId { entityId });
    SpriteManager::addPlayerSprite(registry, entity, posX, posY, 1.5f);
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
    SpriteManager::addProjectileSprite(registry, projectile, posX, posY, 1.5f);
}

void Game::createPlatformFromMessage(const Message& msg, Registry& registry,
    uint32_t entityId, float posX, float posY)
{
    float width = msg.readFloat();
    (void)width;
    float height = msg.readFloat();
    (void)height;
    float offsetX = msg.readFloat();
    (void)offsetX;
    float offsetY = msg.readFloat();
    (void)offsetY;

    Entity platform = factories::createOneWayPlatform(registry,
        posX,
        posY);

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
    SpriteManager::addEnemySprite(registry, enemy, posX, posY, 2.0f);
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
        SpriteManager::addProjectileSprite(registry, entity, position.x, position.y, 1.5f);
        break;
    case EntityType::PLATFORM:
        SpriteManager::addPlatformSprite(registry, entity, position.x, position.y);
        break;
    case EntityType::ENEMY:
        SpriteManager::addEnemySprite(registry, entity, position.x, position.y, 2.0f);
        break;
    case EntityType::POWERUP:
        // PowerUp sprite attachment - could be based on powerup type
        break;
    case EntityType::BOSS:
        // Boss sprite attachment - could be different from enemy
        SpriteManager::addEnemySprite(registry, entity, position.x, position.y, 3.0f);
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

    updateNonPredictedEntities(msg, registry, numPlayers);
}

void Game::updateNonPredictedEntities(const Message& msg, Registry& registry,
    uint8_t numPlayers)
{
    for (uint8_t i = 0; i < numPlayers; ++i) {
        updateSingleEntity(msg, registry);
    }
}

void Game::updateSingleEntity(const Message& msg, Registry& registry)
{
    uint32_t entityId = msg.readU32();
    float posX = msg.readFloat();
    float posY = msg.readFloat();
    uint32_t health = msg.readU32();

    Entity entity = findEntityByServerId(registry, entityId);
    if (entity.id == 0) {
        return;
    }

    updateEntityState(registry, entity, posX, posY, health);
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

void Game::updateEntityState(Registry& registry, Entity entity,
    float posX, float posY, uint32_t health)
{
    updateEntityPosition(registry, entity, posX, posY);
    updateEntityHealth(registry, entity, health);
}

void Game::updateEntityPosition(Registry& registry, Entity entity, float posX, float posY)
{
    if (registry.has<Position>(entity)) {
        auto& position = registry.get<Position>(entity);
        position.v.x = posX;
        position.v.y = posY;
    }

    if (registry.has<Sprite>(entity)) {
        auto& sprite = registry.get<Sprite>(entity);
        sprite.dstRect.x = static_cast<int>(posX);
        sprite.dstRect.y = static_cast<int>(posY);
    }
}

void Game::updateEntityHealth(Registry& registry, Entity entity, uint32_t health)
{
    if (registry.has<Health>(entity)) {
        auto& healthComp = registry.get<Health>(entity);
        healthComp.hp = static_cast<int>(health);
    }
}

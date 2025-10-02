/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game Entity Deserializer - Handles entity creation from network messages
*/

#include "Game.hpp"
#include "ecs/components/AllComponents.hpp"
#include "entities/enemies/CreateEnemy.hpp"
#include "entities/platform/CreatePlatform.hpp"
#include "entities/player/CreatePlayer.hpp"
#include "entities/projectile/CreateProjectile.hpp"
#include <iostream>

void Game::deserializeAndCreateEntity(const Message& msg, Registry& registry)
{
    const_cast<Message&>(msg).resetReadPosition();

    uint32_t entityId = msg.readU32();
    uint8_t entityType = msg.readU8();
    float posX = msg.readFloat();
    float posY = msg.readFloat();

    switch (entityType) {
    case 0:
        createPlayerFromMessage(msg, registry, entityId, posX, posY);
        break;
    case 1:
        createProjectileFromMessage(msg, registry, entityId, posX, posY);
        break;
    case 2:
        createPlatformFromMessage(msg, registry, entityId, posX, posY);
        break;
    case 3:
        createEnemyFromMessage(msg, registry, entityId, posX, posY);
        break;
    default:
        logUnknownEntityType(entityType);
        break;
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

    Entity entity = factories::createPlayer(registry,
        Position { posX, posY },
        Health { static_cast<int>(healthValue) },
        Hitbox { width, height, offsetX, offsetY });

    registry.add<ServerEntityId>(entity, ServerEntityId { entityId });
    addPlayerSprite(registry, entity, posX, posY);
}

void Game::addPlayerSprite(Registry& registry, Entity entity, float posX, float posY)
{
    Sprite sprite;
    sprite.texture_id = "player_sprite.png";
    sprite.frame_width = 32;
    sprite.frame_height = 32;
    sprite.srcRect = { 0, 0, 32, 32 };
    sprite.dstRect = { static_cast<int>(posX), static_cast<int>(posY), 32, 32 };
    registry.add<Sprite>(entity, sprite);
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
    uint32_t ownerId = msg.readU32();
    float lifetimeValue = msg.readFloat();

    factories::createProjectile(registry,
        Position { posX, posY },
        Velocity { velX, velY },
        Damage { damageValue },
        Hitbox { width, height, offsetX, offsetY },
        Parent { Entity { ownerId, 0 } },
        Lifetime { lifetimeValue });
}

void Game::createPlatformFromMessage(const Message& msg, Registry& registry,
    uint32_t entityId, float posX, float posY)
{
    float width = msg.readFloat();
    float height = msg.readFloat();
    float offsetX = msg.readFloat();
    float offsetY = msg.readFloat();

    factories::createPlatform(registry,
        Position { posX, posY },
        Hitbox { width, height, offsetX, offsetY });
}

void Game::createEnemyFromMessage(const Message& msg, Registry& registry,
    uint32_t entityId, float posX, float posY)
{
    uint32_t healthValue = msg.readU32();
    float width = msg.readFloat();
    float height = msg.readFloat();
    float offsetX = msg.readFloat();
    float offsetY = msg.readFloat();

    Entity enemy = factories::createEnemy(
        registry,
        Position { posX, posY },
        Health { static_cast<int>(healthValue) },
        Hitbox { width, height, offsetX, offsetY });

    registry.add<ServerEntityId>(enemy, ServerEntityId { entityId });
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
        position.x = posX;
        position.y = posY;
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

/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Entity Metadata Registration - Registers metadata for all entity types
*/

#include "../include/EntityData.hpp"
#include "../include/EntityFactory.hpp"
#include "../include/EntityMetadata.hpp"
#include "../include/EntityType.hpp"
#include "ecs/Registry.hpp"
#include "ecs/components/Damage.hpp"
#include "ecs/components/Health.hpp"
#include "ecs/components/Hitbox.hpp"
#include "ecs/components/Lifetime.hpp"
#include "ecs/components/Parent.hpp"
#include "ecs/components/Position.hpp"
#include "ecs/components/PowerUp.hpp"
#include "ecs/components/TextBox.hpp"
#include "ecs/components/Velocity.hpp"
#include "entities/enemies/CreateEnemy.hpp"
#include "entities/boss/CreateBoss.hpp"
#include "entities/platform/CreatePlatform.hpp"
#include "entities/player/CreatePlayer.hpp"
#include "entities/powerUp/CreatePowerUp.hpp"
#include "entities/projectile/CreateProjectile.hpp"
#include <stdexcept>

// Forward declarations for factory functions
Entity createPlayerFromData(class Registry& registry, const class EntityData& data);
Entity createProjectileFromData(class Registry& registry, const class EntityData& data);
Entity createPlatformFromData(class Registry& registry, const class EntityData& data);
Entity createEnemyFromData(class Registry& registry, const class EntityData& data);
Entity createPowerUpFromData(class Registry& registry, const class EntityData& data);
Entity createBossFromData(class Registry& registry, const class EntityData& data);

void initializeEntityMetadataRegistration()
{
    auto& registry = EntityMetadataRegistry::getInstance();
    auto& factory = EntityFactory::getInstance();

    // Player Entity Metadata
    registry.registerMetadata(EntityMetadata {
        EntityType::PLAYER,
        "Player",
        { { "position", PropertyType::VEC2_FLOAT, true },
            { "health", PropertyType::INT, true },
            { "hitbox", PropertyType::VEC4_FLOAT, true },
            { "player_id", PropertyType::UINT32, true },
            { "username", PropertyType::STRING, true } },
        createPlayerFromData });
    factory.registerFactory(EntityType::PLAYER, createPlayerFromData);

    // Projectile Entity Metadata
    registry.registerMetadata(EntityMetadata {
        EntityType::PROJECTILE,
        "Projectile",
        { { "position", PropertyType::VEC2_FLOAT, true },
            { "velocity", PropertyType::VEC2_FLOAT, true },
            { "damage", PropertyType::FLOAT, true },
            { "hitbox", PropertyType::VEC4_FLOAT, true },
            { "parent", PropertyType::ENTITY_ID, true },
            { "lifetime", PropertyType::FLOAT, true } },
        createProjectileFromData });
    factory.registerFactory(EntityType::PROJECTILE, createProjectileFromData);

    // Platform Entity Metadata
    registry.registerMetadata(EntityMetadata {
        EntityType::PLATFORM,
        "Platform",
        { { "position", PropertyType::VEC2_FLOAT, true },
            { "velocity", PropertyType::VEC2_FLOAT, true },
            { "hitbox", PropertyType::VEC4_FLOAT, true } },
        createPlatformFromData });
    factory.registerFactory(EntityType::PLATFORM, createPlatformFromData);

    // Enemy Entity Metadata
    registry.registerMetadata(EntityMetadata {
        EntityType::ENEMY,
        "Enemy",
        { { "position", PropertyType::VEC2_FLOAT, true },
            { "velocity", PropertyType::VEC2_FLOAT, true },
            { "health", PropertyType::INT, true },
            { "hitbox", PropertyType::VEC4_FLOAT, true } },
        createEnemyFromData });
    factory.registerFactory(EntityType::ENEMY, createEnemyFromData);

    // PowerUp Entity Metadata
    registry.registerMetadata(EntityMetadata {
        EntityType::POWERUP,
        "PowerUp",
        { { "position", PropertyType::VEC2_FLOAT, true },
            { "powerup_type", PropertyType::POWERUP_TYPE, true },
            { "effect_duration", PropertyType::FLOAT, true },
            { "hitbox", PropertyType::VEC4_FLOAT, true },
            { "lifetime", PropertyType::FLOAT, true } },
        createPowerUpFromData });
    factory.registerFactory(EntityType::POWERUP, createPowerUpFromData);

    // Boss Entity Metadata (extends Enemy)
    registry.registerMetadata(EntityMetadata {
        EntityType::BOSS,
        "Boss",
        { { "position", PropertyType::VEC2_FLOAT, true },
            { "velocity", PropertyType::VEC2_FLOAT, true },
            { "health", PropertyType::INT, true },
            { "hitbox", PropertyType::VEC4_FLOAT, true } },
        createBossFromData });
    factory.registerFactory(EntityType::BOSS, createBossFromData);
}

Entity createPlayerFromData(Registry& registry, const EntityData& data)
{
    Vector2 position = data.get<Vector2>("position");
    int health = data.get<int>("health");
    std::array<float, 4> hitbox = data.get<std::array<float, 4>>("hitbox");
    std::string username = data.get<std::string>("username");
    auto player_id = data.get<uint32_t>("player_id");

    return factories::createPlayer(
        registry,
        username,
        Position { position.x, position.y },
        Health { health },
        Hitbox { hitbox[0], hitbox[1], hitbox[2], hitbox[3] },
        player_id);
}

Entity createProjectileFromData(class Registry& registry, const class EntityData& data)
{
    Vector2 position = data.get<Vector2>("position");
    Vector2 velocity = data.get<Vector2>("velocity");
    float damage = data.get<float>("damage");
    std::array<float, 4> hitbox = data.get<std::array<float, 4>>("hitbox");
    Entity parent = data.get<Entity>("parent");
    float lifetime = data.get<float>("lifetime");

    return factories::createProjectile(registry,
        Position { position.x, position.y },
        Velocity { velocity.x, velocity.y },
        Damage { damage },
        Hitbox { hitbox[0], hitbox[1], hitbox[2], hitbox[3] },
        Parent { parent },
        Lifetime { lifetime });
}

Entity createPlatformFromData(class Registry& registry, const class EntityData& data)
{
    Vector2 position = data.get<Vector2>("position");
    Vector2 velocity = data.get<Vector2>("velocity");
    std::array<float, 4> hitbox = data.get<std::array<float, 4>>("hitbox");

    return factories::createOneWayPlatform(
        registry,
        position.x,
        position.y,
        velocity.x,
        velocity.y,
        hitbox[0],
        hitbox[1],
        hitbox[2],
        hitbox[3]);
}

Entity createEnemyFromData(class Registry& registry, const class EntityData& data)
{
    Vector2 position = data.get<Vector2>("position");
    Vector2 velocity = data.get<Vector2>("velocity");
    int health = data.get<int>("health");
    std::array<float, 4> hitbox = data.get<std::array<float, 4>>("hitbox");

    return factories::createEnemy(registry,
        Position { position.x, position.y },
        Health { health },
        Hitbox { hitbox[0], hitbox[1], hitbox[2], hitbox[3] },
        Velocity { velocity.x, velocity.y });
}

Entity createPowerUpFromData(class Registry& registry, const class EntityData& data)
{
    Vector2 position = data.get<Vector2>("position");
    PowerUpType powerupType = data.get<PowerUpType>("powerup_type");
    float effectDuration = data.get<float>("effect_duration");
    std::array<float, 4> hitbox = data.get<std::array<float, 4>>("hitbox");
    float lifetime = data.get<float>("lifetime");

    return factories::createPowerUp(registry,
        Position { position.x, position.y },
        Velocity { 0.0f, 0.0f }, // Stationary
        Hitbox { hitbox[0], hitbox[1], hitbox[2], hitbox[3] },
        Lifetime { lifetime },
        powerupType,
        effectDuration);
}

Entity createBossFromData(class Registry& registry, const class EntityData& data)
{
    // Boss uses same factory as enemy but with potentially different properties
    Vector2 position = data.get<Vector2>("position");
    Vector2 velocity = data.get<Vector2>("velocity");
    int health = data.get<int>("health");
    std::array<float, 4> hitbox = data.get<std::array<float, 4>>("hitbox");

    return factories::createBoss(registry,
        Position { position.x, position.y },
        Health { health },
        Hitbox { hitbox[0], hitbox[1], hitbox[2], hitbox[3] },
        Velocity { velocity.x, velocity.y });
}

/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game class implementation
*/

#include "Game.hpp"
#include "ecs/components/AllComponents.hpp"
#include "ecs/systems/CollisionSystem.hpp"
#include "ecs/systems/GravitySystem.hpp"
#include "ecs/systems/MovementSystem.hpp"
#include "entities/platform/CreatePlatform.hpp"
#include "entities/player/CreatePlayer.hpp"
#include "entities/player/HandlePlayerInputs.hpp"
#include "systems/RenderSystem.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include <thread>

Game::Game()
    : _graphics(GraphicsManager::getInstance())
    , _inputs(InputManager::getInstance())
    , m_clientNetwork(std::make_unique<Client::RTypeClient>("127.0.0.1", 4242, 2020, m_events))
    , _isRunning(false)
{
    m_clientNetwork->connectToServerRequest();
}

Game::~Game()
{
    cleanup();
}

bool Game::initialize()
{
    if (!_graphics.initialize("R-Type - ECS + SDL2 Demo", 800, 600)) {
        std::cerr << "Failed to initialize graphics!" << std::endl;
        return false;
    }

    // Entities will be created dynamically via SPAWN_ENTITY messages from server
    // No hardcoded entities needed - client receives all entities from server

    _timer.start();
    _isRunning = true;
    return true;
}

void Game::run()
{
    if (!_isRunning) {
        std::cerr << "Game not initialized! Call initialize() first." << std::endl;
        return;
    }
    auto netThread = std::thread([this]() { m_clientNetwork->start(); });
    SDL_Event event;

    while (_isRunning) {
        processNetworkEvents();
        float deltaTime = _timer.getDeltaTime();

        _inputs.updateInputs(event);
        if (_inputs.isActionPressed(GameAction::QUIT)) {
            _isRunning = false;
            m_clientNetwork->disconnectFromServerRequest();
            break;
        }
        update(deltaTime);
        render();

        SDL_Delay(16); // ~60 FPS
    }

    if (netThread.joinable()) {
        netThread.join();
    }
    std::cout << "Game loop ended." << std::endl;
}

void Game::update(float deltaTime)
{
    m_clientNetwork->handleInputs(_inputs);
    handlePlayerInputs(_inputs, _registry);
    gravitySystem(_registry, deltaTime);
    movementSystem(_registry, deltaTime);
    collisionSystem(_registry, deltaTime);
}

void Game::render()
{
    renderSystem(_registry);
}

void Game::cleanup()
{
    if (_isRunning) {
        _graphics.cleanup();
        _isRunning = false;
        std::cout << "Game cleanup completed." << std::endl;
    }
    if (m_clientNetwork) {
        m_clientNetwork->stop();
    }
}

void Game::processNetworkEvents()
{
    const auto event = m_events.pop();
    if (!event)
        return;
    const auto value = event.value();

    switch (value.type) {
    case MessageType::CONNECT_ACK:
        std::cout << "Connection acknowledged by server" << std::endl;
        break;
    case MessageType::SPAWN_ENTITY:
        if (std::holds_alternative<Message>(value.payload)) {
            const Message& msg = std::get<Message>(value.payload);
            deserializeAndCreateEntity(msg, _registry);
        }
        break;
    default:
        break;
    }
}

void Game::deserializeAndCreateEntity(const Message& msg, Registry& registry)
{
    const_cast<Message&>(msg).resetReadPosition();

    uint32_t entityId = msg.readU32();
    uint8_t entityType = msg.readU8();

    float posX = msg.readFloat();
    float posY = msg.readFloat();

    switch (entityType) {
    case 0: { // Player
        uint32_t healthValue = msg.readU32();
        float width = msg.readFloat();
        float height = msg.readFloat();
        float offsetX = msg.readFloat();
        float offsetY = msg.readFloat();

        Entity entity = factories::createPlayer(registry,
            Position { posX, posY },
            Health { static_cast<int>(healthValue) },
            Hitbox { width, height, offsetX, offsetY });

        // TODO : AUTO Sprite component for rendering
        Sprite sprite;
        sprite.texture_id = "player_sprite.png";
        sprite.frame_width = 32;
        sprite.frame_height = 32;
        sprite.srcRect = { 0, 0, 32, 32 };
        sprite.dstRect = { static_cast<int>(posX), static_cast<int>(posY), 32, 32 };
        registry.add<Sprite>(entity, sprite);
        break;
    }

    case 1: { // Projectile
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
            OwnerId { static_cast<int>(ownerId) },
            Lifetime { lifetimeValue });
        break;
    }

    case 2: { // Platform
        float width = msg.readFloat();
        float height = msg.readFloat();
        float offsetX = msg.readFloat();
        float offsetY = msg.readFloat();

        factories::createPlatform(registry,
            Position { posX, posY },
            Hitbox { width, height, offsetX, offsetY });
        break;
    }

    case 3: { // Enemy - TODO: Create enemy factory
        std::cout << "Enemy entity creation not implemented yet" << std::endl;
        break;
    }

    default:
        std::cout << "Unknown entity type: " << static_cast<int>(entityType) << std::endl;
        break;
    }

    std::cout << "Created entity ID " << entityId << " of type " << static_cast<int>(entityType)
              << " at position (" << posX << ", " << posY << ")" << std::endl;
}

/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game class implementation
*/

#include "Game.hpp"
#include "IpEncoding.hpp"
#include "ecs/components/AllComponents.hpp"
#include "ecs/systems/CollisionSystem.hpp"
#include "ecs/systems/GravitySystem.hpp"
#include "ecs/systems/MovementSystem.hpp"
#include "ecs/systems/ProjectileSystem.hpp"
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
    , m_clientNetwork(nullptr)
    , _isRunning(false)
{
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

    _timer.start();
    _lastTickTime = std::chrono::steady_clock::now();
    _accumulatedTime = 0.0f;
    _isRunning = true;
    _state = GameState::MENU;
    m_menu.activate(Menu::Page::Connect);

    return true;
}

void Game::run()
{

    // TODO:: Refactor
    if (!_isRunning) {
        std::cerr << "Game not initialized! Call initialize() first." << std::endl;
        return;
    }

    SDL_Event event;

    while (_isRunning) {
        processNetworkEvents();
        float deltaTime = _timer.getDeltaTime();

        _inputs.beginFrame();
        while (SDL_PollEvent(&event)) {
            _inputs.handleSDLEvent(event);
            if (_state == GameState::MENU) {
                m_menu.handleEvent(event);
            }
        }

        if (_inputs.isActionPressed(GameAction::QUIT)) {
            _isRunning = false;
            break;
        }

        if (_state == GameState::MENU) {
            if (m_menu.popConnectRequest()) {
                std::string ip;
                int port;
                decodeIp(m_menu.m_inputCode, ip, port);
                printf("Decoded IP: %s, Port: %d\n", ip.c_str(), port);
                m_clientNetwork = std::make_unique<Client::RTypeClient>(ip, port, 2020, m_events);
                m_networkThread = std::thread([this]() { m_clientNetwork->start(); });
                m_clientNetwork->connectToServerRequest();
                m_menu.onConnected();
            }

            // Page 2: Lobby
            if (m_menu.popCreateLobbyRequest()) {
                m_clientNetwork->createLobbyRequest();
                m_menu.onCreated();
                printf("Lobby created, waiting for game start...\n");
            }
            if (m_menu.popJoinLobbyRequest()) {
                m_clientNetwork->joinLobbyRequest(std::stoi(m_menu.m_inputCode));
                m_menu.deactivate(_registry);
                startGameplay();
            }

            // Page 3: Start -> button "Start"
            if (m_menu.popStartRequest()) {
                m_clientNetwork->startGameRequest();
                m_menu.deactivate(_registry);
                startGameplay();
            }

            buttonSystem(_registry);
            m_menu.render(_graphics, _registry);
            SDL_Delay(16);
            continue;
        }

        update(deltaTime);
        render();
        SDL_Delay(16);
    }

    std::cout << "Game loop ended." << std::endl;
}

void Game::update(float deltaTime)
{
    _accumulatedTime += deltaTime;

    while (_accumulatedTime >= TICK_DURATION) {
        m_clientNetwork->handleInputs(_inputs);
        m_clientNetwork->incrementTick();
        handlePlayerInputs(_inputs, _registry);
        gravitySystem(_registry, TICK_DURATION);
        movementSystem(_registry, TICK_DURATION);
        projectileSystem(_registry, TICK_DURATION);
        collisionSystem(_registry, TICK_DURATION);
        buttonSystem(_registry);

        _accumulatedTime -= TICK_DURATION;
    }
}

void Game::render()
{
    renderSystem(_registry);
}

void Game::cleanup()
{
    if (_isRunning) {
        SDL_StopTextInput();
        _graphics.cleanup();
        _isRunning = false;
        std::cout << "Game cleanup completed." << std::endl;
    }
    if (m_clientNetwork) {
        m_clientNetwork->stop();
    }
    if (m_networkThread.joinable()) {
        m_networkThread.join();
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
    case MessageType::GAME_STATE:
        if (std::holds_alternative<Message>(value.payload)) {
            const Message& msg = std::get<Message>(value.payload);
            deserializeAndUpdateGameState(msg, _registry);
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
    case 0: {
        uint32_t healthValue = msg.readU32();
        float width = msg.readFloat();
        float height = msg.readFloat();
        float offsetX = msg.readFloat();
        float offsetY = msg.readFloat();

        Entity entity = factories::createPlayer(registry,
            Position { posX, posY },
            Health { static_cast<int>(healthValue) },
            Hitbox { width, height, offsetX, offsetY });

        registry.add<ServerEntityId>(entity, ServerEntityId { entityId });

        // TODO : AUTO Sprite component for rendering // DONE: on branch Ressource_manager
        Sprite sprite;
        sprite.texture_id = "player_sprite.png";
        sprite.frame_width = 32;
        sprite.frame_height = 32;
        sprite.srcRect = { 0, 0, 32, 32 };
        sprite.dstRect = { static_cast<int>(posX), static_cast<int>(posY), 32, 32 };
        registry.add<Sprite>(entity, sprite);
        break;
    }

    case 1: {
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

    case 2: {
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

void Game::deserializeAndUpdateGameState(const Message& msg, Registry& registry)
{
    const_cast<Message&>(msg).resetReadPosition();

    uint32_t tick = msg.readU32();
    uint8_t numPlayers = msg.readU8();

    for (uint8_t i = 0; i < numPlayers; ++i) {
        uint32_t entityId = msg.readU32();
        float posX = msg.readFloat();
        float posY = msg.readFloat();
        uint32_t health = msg.readU32();

        auto view = registry.view<ServerEntityId>();
        for (auto it = view.begin(); it != view.end(); ++it) {
            Entity entity = it.entity();
            const auto& serverId = registry.get<ServerEntityId>(entity);
            if (serverId.id == entityId) {
                if (registry.has<Position>(entity)) {
                    auto& position = registry.get<Position>(entity);
                    position.x = posX;
                    position.y = posY;
                }

                if (registry.has<Health>(entity)) {
                    auto& healthComp = registry.get<Health>(entity);
                    healthComp.hp = static_cast<int>(health);
                }

                if (registry.has<Sprite>(entity)) {
                    auto& sprite = registry.get<Sprite>(entity);
                    sprite.dstRect.x = static_cast<int>(posX);
                    sprite.dstRect.y = static_cast<int>(posY);
                }

                break;
            }
        }
    }
}

void Game::startGameplay()
{
    _state = GameState::PLAYING;
}

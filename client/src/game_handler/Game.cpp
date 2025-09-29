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

    Entity playerEntity = factories::createPlayer(_registry);
    (void)playerEntity;

    factories::createOneWayPlatform(_registry, 100, 400);
    factories::createPlatform(_registry, 300, 350);
    factories::createPlatform(_registry, 500, 300);
    factories::createOneWayPlatform(_registry, 200, 250);

    for (int i = 0; i < 8; i++) {
        factories::createPlatform(_registry, i * 100, 520);
    }

    _timer.start();
    _isRunning = true;
    _state = GameState::MENU;
    m_menu.activate();
    return true;
}

void Game::run()
{
    if (!_isRunning) {
        std::cerr << "Game not initialized! Call initialize() first." << std::endl;
        return;
    }

    SDL_Event event;

    while (_isRunning) {
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
            // Page 1: Connect -> passe à la page Lobby (hard-coded)
            if (m_menu.popConnectRequest()) {
                m_menu.onConnected();
            }

            // Page 2: Lobby -> Create/Join -> démarre le jeu (hard-coded)
            bool goPlay = false;
            if (m_menu.popCreateLobbyRequest())
                goPlay = true;
            if (m_menu.popJoinLobbyRequest())
                goPlay = true;

            if (goPlay) {
                m_menu.deactivate(_registry); // détruit les entités boutons
                startGameplay();
            }

            // MAJ ECS boutons + rendu
            buttonSystem(_registry); // [client/src/ecs/systems/ButtonSystem.hpp](client/src/ecs/systems/ButtonSystem.hpp)
            m_menu.render(_graphics, _registry);
            SDL_Delay(16);
            continue;
        }

        // Etat PLAYING
        update(deltaTime);
        render();
        SDL_Delay(16);
    }

    std::cout << "Game loop ended." << std::endl;
}

void Game::update(float deltaTime)
{
    handlePlayerInputs(_inputs, _registry);
    gravitySystem(_registry, deltaTime);
    movementSystem(_registry, deltaTime);
    collisionSystem(_registry, deltaTime);
    buttonSystem(_registry);
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
    default:
        break;
    }
}

void Game::startGameplay()
{
    _state = GameState::PLAYING;
}

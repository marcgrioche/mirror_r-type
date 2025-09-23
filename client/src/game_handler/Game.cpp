/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game class implementation
*/

#include "Game.hpp"
#include "ecs/Components.hpp"
#include "ecs/systems/MovementSystem.hpp"
#include "ecs/systems/RenderSystem.hpp"
#include "player/CreatePlayer.hpp"
#include "player/HandlePlayerInputs.hpp"
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
    factories::createPlayer(_registry);
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
    handlePlayerInputs(_inputs, _registry);
    movementSystem(_registry, deltaTime);
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

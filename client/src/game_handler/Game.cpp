/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game class implementation
*/

#include "Game.hpp"
#include "components/AllComponents.hpp"
#include "systems/MovementSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "entities/player/CreatePlayer.hpp"
#include "entities/player/HandlePlayerInputs.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include "platform/CreatePlatform.hpp"

Game::Game()
    : _graphics(GraphicsManager::getInstance())
    , _inputs(InputManager::getInstance())
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
    factories::createPlayer(_registry);
    factories::createOneWayPlatform(_registry, 100, 100);
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
    SDL_Event event;

    while (_isRunning) {
        float deltaTime = _timer.getDeltaTime();

        _inputs.updateInputs(event);
        if (_inputs.isActionPressed(GameAction::QUIT)) {
            _isRunning = false;
            break;
        }
        update(deltaTime);
        render();

        SDL_Delay(16); // ~60 FPS
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
}

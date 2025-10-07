/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game class implementation - Core initialization and main loop
*/

#include "Game.hpp"
#include "ButtonSystem.hpp"
#include <SDL.h>
#include <iostream>

Game::Game(bool isLocalMode, uint16_t clientPort)
    : _registry(std::make_shared<Registry>())
    , _graphics(GraphicsManager::getInstance())
    , _inputs(InputManager::getInstance())
    , m_clientNetwork(nullptr)
    , m_localGameInstance(nullptr)
    , _isRunning(false)
    , m_isLocalMode(isLocalMode)
    , m_clientPort(clientPort)
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

    initializeGameMode();
    return true;
}

void Game::initializeGameMode()
{
    if (m_isLocalMode) {
        initializeLocalMode();
    } else {
        initializeMenuMode();
    }
}

void Game::initializeLocalMode()
{
    m_localGameInstance = std::make_unique<GameInstance>(0);
    m_localGameInstance->initialize();
    m_localGameInstance->addPlayer(1);

    _state = GameState::PLAYING;
    std::cout << "Local mode initialized - starting gameplay directly" << std::endl;
}

void Game::initializeMenuMode()
{
    _state = GameState::MENU;
    m_menu.activate(Menu::Page::Connect);
}

// TODO: send a disconnect message to the server when closing the window when
// the player is connected
void Game::run()
{
    if (!_isRunning) {
        std::cerr << "Game not initialized! Call initialize() first." << std::endl;
        return;
    }

    SDL_Event event;

    while (_isRunning) {
        processGameMode();

        float deltaTime = _timer.getDeltaTime();
        handleInputEvents(event);

        if (_inputs.isActionPressed(GameAction::QUIT)) {
            _isRunning = false;
            break;
        }

        if (_state == GameState::MENU) {
            runMenuLoop();
            continue;
        }

        runGameLoop(deltaTime);
    }

    std::cout << "Game loop ended." << std::endl;
}

void Game::processGameMode()
{
    if (m_isLocalMode) {
        processLocalGameUpdates();
    } else {
        processNetworkEvents();
    }
}

void Game::handleInputEvents(SDL_Event& event)
{
    _inputs.beginFrame();
    while (SDL_PollEvent(&event)) {
        _inputs.handleSDLEvent(event);
        if (_state == GameState::MENU) {
            m_menu.handleEvent(event);
        }
    }
}

void Game::runMenuLoop()
{
    processMenuEvents();
    buttonSystem(_registry);
    m_menu.render(_graphics, _registry);
    SDL_Delay(16);
}

void Game::runGameLoop(float deltaTime)
{
    update(deltaTime);
    render();
    SDL_Delay(16);
}

void Game::cleanup()
{
    if (_isRunning) {
        SDL_StopTextInput();
        _graphics.cleanup();
        _isRunning = false;
        std::cout << "Game cleanup completed." << std::endl;
    }

    cleanupNetwork();
}

void Game::cleanupNetwork()
{
    if (m_clientNetwork) {
        m_clientNetwork->stop();
    }
    if (m_networkThread.joinable()) {
        m_networkThread.join();
    }
}

/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game class implementation - Core initialization and main loop
*/

#include "Game.hpp"
#include "ButtonSystem.hpp"
#include "IpEncoding.hpp" // Ajoute cet include pour decodeIp
#include "managers/EventManager.hpp" // Ajoute cet include
#include "managers/ResourceManager.hpp"
#include <SDL.h>
#include <iostream>

Game::Game(bool isLocalMode, uint16_t clientPort)
    : _graphics(GraphicsManager::getInstance())
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

    auto& resourceManager = ResourceManager::getInstance();
    SDL_Renderer* renderer = _graphics.getRenderer();

    if (!resourceManager.loadTexture(renderer, "player_sprite.png", "client/res/sprites/player_sprite.png")) {
        std::cout << "Warning: Failed to load player sprite texture - using fallback rectangles" << std::endl;
    }

    if (!resourceManager.loadTexture(renderer, "eye_spritesheet.png", "client/res/sprites/eye_spritesheet.png")) {
        std::cout << "Warning: Failed to load eye spritesheet texture - using fallback rectangles" << std::endl;
    }

    if (!resourceManager.loadTexture(renderer, "WallOfFlesh.png", "client/res/sprites/WallOfFlesh.png")) {
        std::cout << "Warning: Failed to load WallOfFlesh texture - using fallback rectangles" << std::endl;
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
    m_menu.activate(_registry, Menu::Page::CONNECTION); // Change Connect -> HOME pour commencer par la page d'accueil
}

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
            m_menu.handleEvent(event, _registry); // Passe registry
        }
    }
}

void Game::runMenuLoop()
{
    // Traite les événements du nouveau système de menu
    processMenuEvents(); // Utilise la version adaptée

    // Met à jour et rendu du menu
    m_menu.update(_registry, _timer.getDeltaTime()); // Passe registry et deltaTime
    buttonSystem(_registry);

    // Traitement des événements
    EventManager::getInstance().processEvents();

    // Rendu
    _graphics.clear(0, 0, 0, 255);
    m_menu.render(_graphics, _registry); // Passe registry
    _graphics.present();

    // Nettoie les événements pour la frame suivante
    EventManager::getInstance().clear();

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
        sendDisconnectMessage();
        m_clientNetwork->stop();
    }
    if (m_networkThread.joinable()) {
        m_networkThread.join();
    }
}

void Game::sendDisconnectMessage()
{
    if (!m_clientNetwork)
        return;

    try {
        Message disconnectMsg(MessageType::DISCONNECT, 0, m_clientNetwork->getPlayerId(), 1);
        m_clientNetwork->sendMessageImmediately(disconnectMsg);
        std::cout << "Disconnect message sent to server" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed to send disconnect message: " << e.what() << std::endl;
    }
}

void Game::processMenuRequests()
{
    // Vérifie les demandes de connexion
    if (m_menu.hasConnectionRequest()) {
        std::string connectionCode = m_menu.getConnectionCode(_registry);
        handleMenuConnectionRequest(connectionCode);
        m_menu.clearAllRequests();
    }

    // Vérifie les demandes de rejoindre un lobby
    if (m_menu.hasJoinRequest()) {
        std::string lobbyCode = m_menu.getJoinCode(_registry);
        std::string pseudo = m_menu.getUserPseudo(_registry);
        handleMenuJoinRequest(lobbyCode, pseudo);
        m_menu.clearAllRequests();
    }

    // Vérifie les demandes de création de lobby
    if (m_menu.hasCreateRequest()) {
        std::string pseudo = m_menu.getUserPseudo(_registry);
        handleMenuCreateRequest(pseudo);
        m_menu.clearAllRequests();
    }
}

void Game::handleMenuConnectionRequest(const std::string& connectionCode)
{
    if (connectionCode.empty()) {
        std::cout << "Please enter a connection code" << std::endl;
        return;
    }

    // Decode IP and port (utilise la logique existante de GameMenuHandler.cpp)
    std::string ip;
    int port;
    decodeIp(connectionCode, ip, port);
    ip = ip.empty() ? "127.0.0.1" : ip;
    port = (port == 0) ? 4242 : port;

    connectToServer(ip, port);
}

void Game::handleMenuJoinRequest(const std::string& lobbyCode, const std::string& pseudo)
{
    if (!m_clientNetwork) {
        std::cout << "ERROR: Not connected to server. Use Connect first." << std::endl;
        return;
    }

    if (lobbyCode.empty()) {
        std::cout << "Please enter a lobby code" << std::endl;
        return;
    }

    try {
        uint32_t lobbyId = static_cast<uint32_t>(std::stoul(lobbyCode));
        m_clientNetwork->joinLobbyRequest(lobbyId);
        std::cout << "Joining lobby " << lobbyId << " as " << pseudo << std::endl;
    } catch (const std::exception& e) {
        std::cout << "ERROR: Invalid lobby code: " << lobbyCode << std::endl;
    }
}

void Game::handleMenuCreateRequest(const std::string& pseudo)
{
    if (!m_clientNetwork) {
        std::cout << "ERROR: Not connected to server. Use Connect first." << std::endl;
        return;
    }

    m_clientNetwork->createLobbyRequest();
    std::cout << "Creating lobby as " << pseudo << std::endl;
}

void Game::connectToServer(const std::string& serverIp, uint16_t serverPort)
{
    std::cout << "Connecting to " << serverIp << ":" << serverPort << std::endl;

    m_clientNetwork = std::make_unique<Client::RTypeClient>(serverIp, serverPort, m_clientPort, m_events);
    m_networkThread = std::thread([this]() { m_clientNetwork->start(); });
    m_clientNetwork->connectToServerRequest();

    // Optionnel: changer vers une page "connecting" ou garder le menu
}

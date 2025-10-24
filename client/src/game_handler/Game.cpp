/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game class implementation - Core initialization and main loop
*/

#include "Game.hpp"
#include "../../shared/include/ComponentMapper.hpp"
#include "../../shared/include/EntityMetadataRegistration.hpp"
#include "ButtonSystem.hpp"
#include "Config.hpp"
#include "IpEncoding.hpp" // Ajoute cet include pour decodeIp
#include "ecs/components/ParallaxState.hpp"
#include "managers/ConfigManager.hpp"
#include "managers/EventManager.hpp" // Ajoute cet include
#include "managers/ResourceManager.hpp"
#include "systems/EyeSystem.hpp"
#include "systems/RenderSystem.hpp"
#include <SDL.h>
#include <fstream>
#include <iostream>

Game::Game(uint16_t clientPort, std::string colorblindType)
    : _graphics(GraphicsManager::getInstance())
    , _inputs(InputManager::getInstance())
    , m_clientNetwork(nullptr)
    , _isRunning(false)
    , m_clientPort(clientPort)
{
    _graphics.setColorblindMode(colorblindType);
}

Game::~Game()
{
    cleanup();
}

bool Game::initialize()
{
    initializeEntityMetadataRegistration();
    initializeComponentMappings();

    auto& resourceManager = ResourceManager::getInstance();
    resourceManager.initialize();

    // Initialize with window size (physical window), not game resolution
    if (!_graphics.initialize("R-Type - ECS + SDL2 Demo", WINDOW_WIDTH, WINDOW_HEIGHT)) {
        std::cerr << "Failed to initialize graphics!" << std::endl;
        return false;
    }
    SDL_Renderer* renderer = _graphics.getRenderer();

    if (!resourceManager.loadTexture(renderer, "player_sprite.png", resourceManager.getAssetPath("sprites/player_sprite.png"))) {
        std::cout << "Warning: Failed to load player sprite texture - using fallback rectangles" << std::endl;
    }

    if (!resourceManager.loadTexture(renderer, "eye_spritesheet.png", resourceManager.getAssetPath("sprites/eye_spritesheet.png"))) {
        std::cout << "Warning: Failed to load eye spritesheet texture - using fallback rectangles" << std::endl;
    }

    if (!resourceManager.loadTexture(renderer, "WallOfFlesh.png", resourceManager.getAssetPath("sprites/WallOfFlesh.png"))) {
        std::cout << "Warning: Failed to load WallOfFlesh texture - using fallback rectangles" << std::endl;
    }

    if (!resourceManager.loadTexture(renderer, "heads_monster_idle.png", "client/res/sprites/Heads_Boss/heads_monster_all_idle.png")) {
        std::cout << "Warning: Failed to load heads_monster_idle texture - using fallback rectangles" << std::endl;
    }

    if (!resourceManager.loadTexture(renderer, "heads_monster_attack.png", "client/res/sprites/Heads_Boss/heads_monster_attack.png")) {
        std::cout << "Warning: Failed to load heads_monster_attack texture - using fallback rectangles" << std::endl;
    }

    if (!resourceManager.loadTexture(renderer, "bydo_flying.png", resourceManager.getAssetPath("sprites/bydo_flying.png"))) {
        std::cout << "Warning: Failed to load bydo_flying texture - using fallback rectangles" << std::endl;
    }

    // Load parallax background textures
    if (!resourceManager.loadTexture(renderer, "TopLayer.png", resourceManager.getAssetPath("sprites/Heads_Boss/ParallaxBackground/TopLayer.png"))) {
        std::cout << "Warning: Failed to load TopLayer texture" << std::endl;
    }
    if (!resourceManager.loadTexture(renderer, "Light.png", resourceManager.getAssetPath("sprites/Heads_Boss/ParallaxBackground/Light.png"))) {
        std::cout << "Warning: Failed to load Light texture" << std::endl;
    }
    if (!resourceManager.loadTexture(renderer, "MiddleLayer.png", resourceManager.getAssetPath("sprites/Heads_Boss/ParallaxBackground/MiddleLayer.png"))) {
        std::cout << "Warning: Failed to load MiddleLayer texture" << std::endl;
    }
    if (!resourceManager.loadTexture(renderer, "DownLayer.png", resourceManager.getAssetPath("sprites/Heads_Boss/ParallaxBackground/DownLayer.png"))) {
        std::cout << "Warning: Failed to load DownLayer texture" << std::endl;
    }
    if (!resourceManager.loadTexture(renderer, "Sky.png", resourceManager.getAssetPath("sprites/Heads_Boss/ParallaxBackground/Sky.png"))) {
        std::cout << "Warning: Failed to load Sky texture" << std::endl;
    }

    // LOAD UI
    if (!resourceManager.loadTexture(renderer, "MenuBackground", resourceManager.getAssetPath("sprites/UI/Background/menuBackground.png"))) {
        std::cout << "Warning: Failed to load MenuBackground texture" << std::endl;
    }
    if (!resourceManager.loadTexture(renderer, "ButtonMouth", resourceManager.getAssetPath("sprites/UI/tools/buttonAsset.png"))) {
        std::cout << "Warning: Failed to load buttonMouth texture" << std::endl;
    }
    if (!resourceManager.loadTexture(renderer, "ButtonEye", resourceManager.getAssetPath("sprites/UI/tools/buttonParamAsset.png"))) {
        std::cout << "Warning: Failed to load buttonEye texture" << std::endl;
    }
    if (!resourceManager.loadTexture(renderer, "zoneText", resourceManager.getAssetPath("sprites/UI/tools/textBoxAsset.png"))) {
        std::cout << "Warning: Failed to load zoneText texture" << std::endl;
    }

    if (!resourceManager.loadTexture(renderer, "eyeOutline", resourceManager.getAssetPath("sprites/UI/tools/eyeOutline.png"))) {
        std::cout << "Warning: Failed to load zoneText texture" << std::endl;
    }
    if (!resourceManager.loadTexture(renderer, "eyePupil", resourceManager.getAssetPath("sprites/UI/tools/eyePupil.png"))) {
        std::cout << "Warning: Failed to load zoneText texture" << std::endl;
    }
    if (!resourceManager.loadTexture(renderer, "eyeOutline2", resourceManager.getAssetPath("sprites/UI/tools/eyeOutline2.png"))) {
        std::cout << "Warning: Failed to load zoneText texture" << std::endl;
    }
    if (!resourceManager.loadTexture(renderer, "eyePupil2", resourceManager.getAssetPath("sprites/UI/tools/eyePupil2.png"))) {
        std::cout << "Warning: Failed to load eyepupil2 texture" << std::endl;
    }

    if (!resourceManager.loadTexture(renderer, "eyeOutline3", resourceManager.getAssetPath("sprites/UI/tools/eyeOutline3.png"))) {
        std::cout << "Warning: Failed to load eyeoutline3 texture" << std::endl;
    }
    if (!resourceManager.loadTexture(renderer, "eyePupil3", resourceManager.getAssetPath("sprites/UI/tools/eyePupil3.png"))) {
        std::cout << "Warning: Failed to load eyepupil3 texture" << std::endl;
    }

    _timer.start();
    _lastTickTime = std::chrono::steady_clock::now();
    _accumulatedTime = 0.0f;
    _isRunning = true;

    m_maxLevel = determineMaxLevel();

    initializeMenuMode();
    return true;
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
        processNetworkEvents();

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

    eyeSystem(_registry, 0);
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
    bool hasParallax = false;
    for (auto [entity] : _registry.view<ParallaxState>()) {
        hasParallax = true;
        break;
    }
    if (!hasParallax) {
        _registry.emplace<ParallaxState>(_registry.create_entity(), ParallaxState { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
    }

    update(deltaTime);
    renderSystem(_registry);
    SDL_Delay(16);
}

void Game::cleanup()
{
    // Save key bindings before cleanup
    ConfigManager& config = ConfigManager::getInstance();
    config.saveKeyBindings();

    if (_isRunning) {
        SDL_StopTextInput();
        _graphics.cleanup();
        _isRunning = false;
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

// This function is never called
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

uint32_t Game::determineMaxLevel()
{
    uint32_t level = 1;
    while (true) {
        std::string levelPath = "shared/res/levels/level" + std::to_string(level) + ".json";
        std::ifstream checkFile(levelPath);
        if (!checkFile.is_open()) {
            return level - 1;
        }
        level++;
    }
}

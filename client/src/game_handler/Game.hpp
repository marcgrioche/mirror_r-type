/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game class header
*/

#pragma once

#include "../../../shared/include/EntityData.hpp"
#include "../../../shared/include/EntityFactory.hpp"
#include "../../../shared/include/EntityType.hpp"
#include "../../../shared/include/GameInstance.hpp"
#include "../../../shared/include/Message.hpp"
#include "../../../shared/src/levels/Level.hpp"
#include "../network/RTypeClient.hpp"
#include "../ui/Menu.hpp"
#include "ButtonSystem.hpp"
#include "GameTimer.hpp"
#include "ecs/Registry.hpp"
#include "ecs/components/Sprite.hpp"
#include "entities/enemies/CreateEnemy.hpp"
#include "entities/platform/CreatePlatform.hpp"
#include "entities/player/CreatePlayer.hpp"
#include "entities/projectile/CreateProjectile.hpp"
#include "managers/GraphicsManager.hpp"
#include "managers/InputManager.hpp"
#include "network/NetworkEventQueue.hpp"
#include <chrono>
#include <thread>

#include <string>

#include "prediction/InputHistory.hpp"

class Game {
    /**
     * @brief Enumeration of possible game states
     */
    enum class GameState {
        MENU, /**< Main menu state */
        PLAYING, /**< Active gameplay state */
        PAUSED, /**< Game paused state */
        GAME_OVER /**< Game ended state */
    };

public:
    static constexpr uint32_t TICKS_PER_SECOND = 60; /**< Number of game ticks per second */
    static constexpr float TICK_DURATION = 1.0f / TICKS_PER_SECOND; /**< Duration of each game tick in seconds */

    /**
     * @brief Constructs a Game instance
     * @param clientPort The port to use for client networking
     * @param colorblindType The colorblind filter type ("protanopia", "deuteranopia", "tritanopia", or empty for no filter)
     */
    Game(uint16_t clientPort = 2020, std::string colorblindType = "");

    /**
     * @brief Destructor for Game
     */
    ~Game();

    /**
     * @brief Initializes the game and all subsystems
     * @return True if initialization succeeded, false otherwise
     */
    bool initialize();

    /**
     * @brief Runs the main game loop
     */
    void run();

    /**
     * @brief Cleans up game resources and shuts down subsystems
     */
    void cleanup();

private:
    // Core initialization and loop helpers

    /**
     * @brief Initializes the game for networked multiplayer mode
     */
    void initializeGameMode();

    /**
     * @brief Initializes the menu system
     */
    void initializeMenuMode();

    /**
     * @brief Processes the main game mode logic
     */
    void processGameMode();

    /**
     * @brief Handles SDL input events
     * @param event The SDL event to process
     */
    void handleInputEvents(SDL_Event& event);

    /**
     * @brief Runs the menu loop with delta time parameter
     */
    void runMenuLoop();

    /**
     * @brief Runs the main game loop
     * @param deltaTime Time elapsed since last update
     */
    void runGameLoop(float deltaTime);

    /**
     * @brief Cleans up network resources
     */
    void cleanupNetwork();

    /**
     * @brief Sends a disconnect message to the server
     */
    void sendDisconnectMessage();

    // Core update

    /**
     * @brief Updates the game state
     * @param deltaTime Time elapsed since last update
     */
    void update(float deltaTime);

    /**
     * @brief Starts the gameplay session
     */
    void startGameplay();

    /**
     * @brief Updates the game tick for networked mode
     */
    void updateNetworkGameTick();

    // Menu handling

    /**
     * @brief Processes pending menu requests from the user
     */
    void processMenuRequests();

    /**
     * @brief Handles a menu connection request
     * @param connectionCode The connection code entered by the user
     */
    void handleMenuConnectionRequest(const std::string& connectionCode);

    /**
     * @brief Handles a menu join lobby request
     * @param lobbyCode The lobby code to join
     * @param pseudo The username entered by the user
     */
    void handleMenuJoinRequest(const std::string& lobbyCode, const std::string& pseudo);

    /**
     * @brief Handles a menu create lobby request
     * @param pseudo The username entered by the user
     */
    void handleMenuCreateRequest(const std::string& pseudo);

    /**
     * @brief Connects to a server at the specified address and port
     * @param serverIp The server IP address
     * @param serverPort The server port number
     */
    void connectToServer(const std::string& serverIp, uint16_t serverPort);

    // Conserver les méthodes existantes du GameMenuHandler.cpp

    /**
     * @brief Processes menu events (legacy method)
     */
    void processMenuEvents();

    /**
     * @brief Handles menu connect request (legacy method)
     */
    void handleMenuConnectRequest();

    /**
     * @brief Handles menu create lobby request (legacy method)
     */
    void handleMenuCreateLobbyRequest();

    /**
     * @brief Handles menu join lobby request (legacy method)
     */
    void handleMenuJoinLobbyRequest();

    /**
     * @brief Handles menu start request (legacy method)
     */
    void handleMenuStartRequest();

    /**
     * @brief Handles menu login request (legacy method)
     */
    void handleMenuLoginRequest();

    // Network event handling
    void processNetworkEvents();
    void handleNetworkEvent(const Client::NetworkEvent& event);
    void handleConnectAck();
    void handleLobbyInfo(const Client::NetworkEvent& event);
    void handleSpawnEntity(const Client::NetworkEvent& event);
    void handleDespawnEntity(const Client::NetworkEvent& event);
    void handleGameState(const Client::NetworkEvent& event);
    void handleGameEndWin();
    void handleGameEndLose();
    void handleUsername(const Client::NetworkEvent& event);
    void handleKickPlayerNotice(const Client::NetworkEvent& event);
    void handleAuthResponse(const Client::NetworkEvent& event);

    // Entity deserialization
    void deserializeAndCreateEntity(const Message& msg, Registry& registry);
    void deserializeAndUpdateGameState(const Message& msg, Registry& registry);

    // Entity creation from messages
    void createPlayerFromMessage(const Message& msg, Registry& registry, uint32_t entityId, float posX, float posY);
    void createProjectileFromMessage(const Message& msg, Registry& registry, uint32_t entityId, float posX, float posY);
    void createPlatformFromMessage(const Message& msg, Registry& registry, uint32_t entityId, float posX, float posY);
    void createEnemyFromMessage(const Message& msg, Registry& registry, uint32_t entityId, float posX, float posY);
    void createPowerUpFromMessage(const Message& msg, Registry& registry, uint32_t entityId, float posX, float posY);

    // Entity creation helpers
    void addPlayerSprite(Registry& registry, Entity entity, float posX, float posY);
    void attachSpriteToEntity(Registry& registry, Entity entity, EntityType type, const EntityData& data);
    void logUnknownEntityType(uint8_t entityType);
    void logEntityCreation(uint32_t entityId, uint8_t entityType, float posX, float posY);

    // Game state updates
    void updateNonPredictedEntities(const Message& msg, Registry& registry, uint8_t numPlayers, uint32_t tick);
    void updateSingleEntity(const Message& msg, Registry& registry, uint32_t tick);
    Entity findEntityByServerId(Registry& registry, uint32_t serverId);
    Entity findEntityByClientId(Registry& registry, uint32_t clientId);
    void updateEntityState(Registry& registry, Entity entity, float posX, float posY, uint32_t health,
        uint32_t serverTick);
    void updateEntityPosition(Registry& registry, Entity entity, float posX, float posY,
        uint32_t serverTick);
    void updateEntityHealth(Registry& registry, Entity entity, uint32_t health);

    // Input handling
    std::vector<std::pair<GameInput, bool>> getCurrentInputs();
    void processLocalInputs(std::vector<std::pair<GameInput, bool>>& inputs);
    void updateSystemsComponents();
    void updatePlayerSprite(Registry& registry, Entity entity, float posX, float posY);

    // Entity cleanup
    void clearGameEntities();

    // Level management
    uint32_t determineMaxLevel();

    Menu m_menu;

    Registry _registry;
    GameTimer _timer;

    GraphicsManager& _graphics;
    InputManager& _inputs;
    Client::NetworkEventQueue m_events;
    std::unique_ptr<Client::RTypeClient> m_clientNetwork;
    std::thread m_networkThread;

    bool _isRunning;
    bool m_connected = false;
    bool m_isConnecting = false;
    float m_connectionTimeout = 0.0f;
    bool m_lobbyCreated = false;
    uint16_t m_clientPort;
    InputHistory m_inputHistory;
    uint32_t m_lobbyOwnerId = 0;
    std::unordered_map<uint32_t, std::string> m_lobbyPlayers;
    uint32_t m_currentLevel = 1;
    uint32_t m_maxLevel = 1;
    Level m_currentLevelData;

    std::chrono::steady_clock::time_point _lastTickTime;
    float _accumulatedTime = 0.0f;
    GameState _state = GameState::MENU;
    bool m_networkStarted = false;

    // Nouvelles méthodes pour les réponses serveur
    void onConnectionSuccess();
    void onLobbyJoined(uint32_t lobbyId);
    void onLobbyCreated(uint32_t lobbyId);
    void onGameStarted();
    void onLoginSuccess();
};

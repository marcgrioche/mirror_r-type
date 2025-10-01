/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game class header
*/

#pragma once

#include "../../../shared/include/GameInstance.hpp"
#include "../../../shared/include/Message.hpp"
#include "../network/RTypeClient.hpp"
#include "../ui/Menu.hpp"
#include "ButtonSystem.hpp"
#include "GameTimer.hpp"
#include "ecs/Registry.hpp"
#include "ecs/components/Sprite.hpp"
#include "entities/platform/CreatePlatform.hpp"
#include "entities/player/CreatePlayer.hpp"
#include "entities/projectile/CreateProjectile.hpp"
#include "managers/GraphicsManager.hpp"
#include "managers/InputManager.hpp"
#include "network/NetworkEventQueue.hpp"
#include <chrono>
#include <thread>

#include <string>

class Game {
    enum class GameState {
        MENU,
        PLAYING,
        PAUSED,
        GAME_OVER
    };

public:
    static constexpr uint32_t TICKS_PER_SECOND = 60;
    static constexpr float TICK_DURATION = 1.0f / TICKS_PER_SECOND;

    Game(bool isLocalMode = false);
    ~Game();

    bool initialize();
    void run();
    void cleanup();

private:
    // Core initialization and loop helpers
    void initializeGameMode();
    void initializeLocalMode();
    void initializeMenuMode();
    void processGameMode();
    void handleInputEvents(SDL_Event& event);
    void runMenuLoop();
    void runGameLoop(float deltaTime);
    void cleanupNetwork();

    // Core update and render
    void update(float deltaTime);
    void render();
    void startGameplay();

    // Game tick updates
    void updateGameTick();
    void updateNetworkGameTick();
    void updateLocalGameTick();

    // Menu handling
    void processMenuEvents();
    void handleMenuConnectRequest();
    void handleMenuCreateLobbyRequest();
    void handleMenuJoinLobbyRequest();
    void handleMenuStartRequest();

    // Network event handling
    void processNetworkEvents();
    void handleNetworkEvent(const Client::NetworkEvent& event);
    void handleConnectAck();
    void handleLobbyInfo(const Client::NetworkEvent& event);
    void handleSpawnEntity(const Client::NetworkEvent& event);
    void handleGameState(const Client::NetworkEvent& event);

    // Local game updates
    void processLocalGameUpdates();
    void processNewLocalEntities();
    void createLocalEntity(Entity entity);

    // Entity deserialization
    void deserializeAndCreateEntity(const Message& msg, Registry& registry);
    void deserializeAndUpdateGameState(const Message& msg, Registry& registry);

    // Entity creation from messages
    void createPlayerFromMessage(const Message& msg, Registry& registry, uint32_t entityId, float posX, float posY);
    void createProjectileFromMessage(const Message& msg, Registry& registry, uint32_t entityId, float posX, float posY);
    void createPlatformFromMessage(const Message& msg, Registry& registry, uint32_t entityId, float posX, float posY);
    void createEnemyFromMessage(const Message& msg, Registry& registry, uint32_t entityId, float posX, float posY);

    // Entity creation helpers
    void addPlayerSprite(Registry& registry, Entity entity, float posX, float posY);
    void logUnknownEntityType(uint8_t entityType);
    void logEntityCreation(uint32_t entityId, uint8_t entityType, float posX, float posY);

    // Game state updates
    void updateNonPredictedEntities(const Message& msg, Registry& registry, uint8_t numPlayers);
    void updateSingleEntity(const Message& msg, Registry& registry);
    Entity findEntityByServerId(Registry& registry, uint32_t serverId);
    void updateEntityState(Registry& registry, Entity entity, float posX, float posY, uint32_t health);
    void updateEntityPosition(Registry& registry, Entity entity, float posX, float posY);
    void updateEntityHealth(Registry& registry, Entity entity, uint32_t health);

    // Input handling
    std::vector<std::pair<GameInput, bool>> getCurrentInputs();

    Menu m_menu;

    Registry _registry;
    GameTimer _timer;

    GraphicsManager& _graphics;
    InputManager& _inputs;
    Client::NetworkEventQueue m_events;
    std::unique_ptr<Client::RTypeClient> m_clientNetwork;
    std::unique_ptr<GameInstance> m_localGameInstance;
    std::thread m_networkThread;

    bool _isRunning;
    bool m_isLocalMode;
    bool m_connected = false;
    bool m_lobbyCreated = false;

    std::chrono::steady_clock::time_point _lastTickTime;
    float _accumulatedTime = 0.0f;
    GameState _state = GameState::MENU;
    bool m_networkStarted = false;
};

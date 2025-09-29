/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game class header
*/

#pragma once

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

    Game();
    ~Game();

    bool initialize();
    void run();
    void cleanup();

private:
    void update(float deltaTime);
    void render();
    void processNetworkEvents();
    void deserializeAndCreateEntity(const Message& msg, Registry& registry);
    void deserializeAndUpdateGameState(const Message& msg, Registry& registry);
    void startGameplay();

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
    bool m_lobbyCreated = false;

    std::chrono::steady_clock::time_point _lastTickTime;
    float _accumulatedTime = 0.0f;
    GameState _state = GameState::MENU;
    bool m_networkStarted = false;
};

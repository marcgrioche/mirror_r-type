/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game class header
*/

#pragma once

#include "../../../shared/include/Message.hpp"
#include "../network/RTypeClient.hpp"
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

    Registry _registry;
    GameTimer _timer;

    GraphicsManager& _graphics;
    InputManager& _inputs;
    Client::NetworkEventQueue m_events;
    std::unique_ptr<Client::RTypeClient> m_clientNetwork;

    bool _isRunning;
    GameState _state = GameState::PLAYING;
    bool m_connected = false;
    bool m_lobbyCreated = false;

    std::chrono::steady_clock::time_point _lastTickTime;
    float _accumulatedTime = 0.0f;
};

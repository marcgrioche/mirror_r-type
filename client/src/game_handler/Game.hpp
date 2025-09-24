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
#include "Registry.hpp"
#include "managers/GraphicsManager.hpp"
#include "managers/InputManager.hpp"
#include "network/NetworkEventQueue.hpp"

class Game {
    enum class GameState {
        MENU,
        PLAYING,
        PAUSED,
        GAME_OVER
    };

public:
    Game();
    ~Game();

    bool initialize();
    void run();
    void cleanup();

private:
    void update(float deltaTime);
    void render();
    void processNetworkEvents();

    Registry _registry;
    GameTimer _timer;

    GraphicsManager& _graphics;
    InputManager& _inputs;
    Client::NetworkEventQueue m_events;
    std::unique_ptr<Client::RTypeClient> m_clientNetwork;

    bool _isRunning;
    GameState _state = GameState::PLAYING;
};

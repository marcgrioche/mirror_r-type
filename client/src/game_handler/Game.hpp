/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game class header
*/

#pragma once

#include "GameTimer.hpp"
#include "ecs/Registry.hpp"
#include "managers/GraphicsManager.hpp"
#include "managers/InputManager.hpp"

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

    Registry _registry;
    GameTimer _timer;

    GraphicsManager& _graphics;
    InputManager& _inputs;

    bool _isRunning;
    GameState _state = GameState::PLAYING;
};

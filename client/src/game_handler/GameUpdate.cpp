/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game Update - Manages game loop updates and rendering
*/

#include "../../../shared/src/ecs/systems/BoundarySystem.hpp"
#include "../../../shared/src/entities/enemies/EnemyMovement.hpp"
#include "ButtonSystem.hpp"
#include "Game.hpp"
#include "ecs/systems/CollisionSystem.hpp"
#include "ecs/systems/GravitySystem.hpp"
#include "ecs/systems/HealthSystem.hpp"
#include "ecs/systems/MovementSystem.hpp"
#include "ecs/systems/ProjectileSystem.hpp"
#include "entities/player/HandlePlayerInputs.hpp"
#include "entities/weapons/HandleWeaponInputs.hpp"
#include "systems/RenderSystem.hpp"
#include <iostream>

void Game::update(float deltaTime)
{
    _accumulatedTime += deltaTime;

    while (_accumulatedTime >= TICK_DURATION) {
        updateGameTick();
        _accumulatedTime -= TICK_DURATION;
    }
}

void Game::updateGameTick()
{
    if (!m_isLocalMode) {
        updateNetworkGameTick();
    } else {
        updateLocalGameTick();
    }

    buttonSystem(_registry);
}

void Game::updateNetworkGameTick()
{
    enemyMovement(_registry, TICK_DURATION);
    movementSystem(_registry, TICK_DURATION);
    auto currentInputs = getCurrentInputs();

    m_clientNetwork->sendCurrentInputState(currentInputs);
    m_clientNetwork->incrementTick();
}

void Game::updateLocalGameTick()
{
    enemyMovement(_registry, TICK_DURATION);
    handlePlayerInputs(_inputs, _registry);
    handleWeaponInputs(_inputs, _registry);
    gravitySystem(_registry, TICK_DURATION);
    movementSystem(_registry, TICK_DURATION);
    projectileSystem(_registry, TICK_DURATION);
    collisionSystem(_registry, TICK_DURATION);
    healthSystem(_registry);
}

void Game::render()
{
    renderSystem(_registry);
}

void Game::startGameplay()
{
    _state = GameState::PLAYING;
}

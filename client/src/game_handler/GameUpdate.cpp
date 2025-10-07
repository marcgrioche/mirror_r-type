/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game Update - Manages game loop updates and rendering
*/

#include "ButtonSystem.hpp"
#include "Game.hpp"
#include "ecs/systems/CollisionSystem.hpp"
#include "ecs/systems/GravitySystem.hpp"
#include "ecs/systems/HealthSystem.hpp"
#include "ecs/systems/MovementSystem.hpp"
#include "ecs/systems/ProjectileSystem.hpp"
#include "entities/enemies/enemyMovement.hpp"
#include "entities/player/HandlePlayerInputs.hpp"
#include "systems/RenderSystem.hpp"
#include <iostream>

#include "PlayerActions.hpp"

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

    buttonSystem(m_localGameInstance->getRegistry());
}

void Game::updateNetworkGameTick()
{
    auto currentInputs = getCurrentInputs();
    // auto &registry = m_localGameInstance->getRegistry();

    // TODO: update the velocity with the client entity
    // PlayerActions::updateVelocity(currentInputs, _registry, playerEntity);
    m_localGameInstance->processPlayerInput(m_clientNetwork->getPlayerId(),
        m_localGameInstance->getCurrentTick(), currentInputs);
    m_localGameInstance->updateGameState(m_clientNetwork->getPlayerId());
    // gravitySystem(registry, TICK_DURATION);
    // movementSystem(registry, TICK_DURATION);
    // projectileSystem(registry, TICK_DURATION);
    // collisionSystem(registry, TICK_DURATION);
    m_clientNetwork->sendCurrentInputState(currentInputs);
    m_clientNetwork->incrementTick();
}

void Game::updateLocalGameTick()
{
    auto registry = m_localGameInstance->getRegistry();

    enemyMovement(registry, TICK_DURATION);
    handlePlayerInputs(_inputs, registry);
    gravitySystem(registry, TICK_DURATION);
    movementSystem(registry, TICK_DURATION);
    projectileSystem(registry, TICK_DURATION);
    collisionSystem(registry, TICK_DURATION);
    healthSystem(registry);
}

void Game::render()
{
    renderSystem(m_localGameInstance->getRegistry());
}

void Game::startGameplay()
{
    _state = GameState::PLAYING;
}

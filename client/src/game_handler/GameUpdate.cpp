/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game Update - Manages game loop updates and rendering
*/

#include "../../../shared/src/ecs/systems/BoundarySystem.hpp"
#include "../../../shared/src/ecs/systems/ColisionPlayerPowerUpSystem.hpp"
#include "../../../shared/src/ecs/systems/PowerUpEffectSystem.hpp"
#include "../../../shared/src/ecs/systems/PowerUpSystem.hpp"
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
    movementSystem(_registry, TICK_DURATION);
    auto currentInputs = getCurrentInputs();

    m_clientNetwork->sendCurrentInputState(currentInputs);
    m_clientNetwork->incrementTick();
}

void Game::updateLocalGameTick()
{
    handlePlayerInputs(_inputs, _registry);
    handleWeaponInputs(_inputs, _registry);
    gravitySystem(_registry, TICK_DURATION);
    movementSystem(_registry, TICK_DURATION);
    projectileSystem(_registry, TICK_DURATION);
    collisionSystem(_registry, TICK_DURATION);
    collisionPlayerPowerUpSystem(_registry, TICK_DURATION);
    powerUpEffectSystem(_registry, TICK_DURATION);
    healthSystem(_registry);
    powerUpSystem(_registry, TICK_DURATION);
}

void Game::render()
{
    renderSystem(_registry);
}

void Game::startGameplay()
{
    _state = GameState::PLAYING;
}

void Game::clearGameEntities()
{
    std::vector<Entity> entitiesToKill;

    // Collect entities with game-related tags
    auto playerView = _registry.view<PlayerTag>();
    for (auto it = playerView.begin(); it != playerView.end(); ++it) {
        entitiesToKill.push_back(it.entity());
    }

    auto enemyView = _registry.view<EnemyTag>();
    for (auto it = enemyView.begin(); it != enemyView.end(); ++it) {
        entitiesToKill.push_back(it.entity());
    }

    auto platformView = _registry.view<PlatformTag>();
    for (auto it = platformView.begin(); it != platformView.end(); ++it) {
        entitiesToKill.push_back(it.entity());
    }

    auto projectileView = _registry.view<ProjectileTag>();
    for (auto it = projectileView.begin(); it != projectileView.end(); ++it) {
        entitiesToKill.push_back(it.entity());
    }

    auto weaponView = _registry.view<WeaponTag>();
    for (auto it = weaponView.begin(); it != weaponView.end(); ++it) {
        entitiesToKill.push_back(it.entity());
    }

    auto powerUpView = _registry.view<PowerUpTag>();
    for (auto it = powerUpView.begin(); it != powerUpView.end(); ++it) {
        entitiesToKill.push_back(it.entity());
    }

    // Kill all collected entities
    for (const auto& entity : entitiesToKill) {
        _registry.kill_entity(entity);
    }

    std::cout << "Cleared " << entitiesToKill.size() << " game entities from registry" << std::endl;
}

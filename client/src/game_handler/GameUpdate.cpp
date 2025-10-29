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
#include "ecs/systems/HealthSystem.hpp"
#include "ecs/systems/MovementSystem.hpp"
#include "ecs/systems/ProjectileSystem.hpp"
#include "ecs/systems/RigidBodySystem.hpp"
#include "ecs/systems/WeaponRotationSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "systems/SpriteAnimationSystem.hpp"
#include <iostream>

#include "DashSystem.hpp"
#include "game_instance/PlayerInputProcessor.hpp"

void Game::update(float deltaTime)
{
    _accumulatedTime += deltaTime;

    // std::cout << "Delta time client: " << deltaTime << std::endl;
    while (_accumulatedTime >= TICK_DURATION) {
        updateNetworkGameTick();
        buttonSystem(_registry);
        _accumulatedTime -= TICK_DURATION;
    }
}

void Game::processLocalInputs(std::vector<std::pair<GameInput, bool>>& inputs)
{
    const auto clientId = m_clientNetwork->getPlayerId();
    const auto playerEntity = findEntityByClientId(_registry, clientId);

    if (inputs.empty())
        return;
    std::vector<Entity> newEntities = {};
    PlayerInputProcessor::processInput(
        _registry,
        playerEntity,
        m_clientNetwork->getCurrentTick(),
        inputs,
        newEntities,
        clientId,
        true);
}

void Game::updatePlayerSprite(Registry& registry, const Entity entity, const float posX, const float posY)
{
    if (registry.has<Sprite>(entity)) {
        auto& sprite = registry.get<Sprite>(entity);
        sprite.dstRect.x = static_cast<int>(posX);
        sprite.dstRect.y = static_cast<int>(posY);
    }
}

void Game::updateSystemsComponents()
{
    const auto playerEntity = findEntityByClientId(_registry, m_clientNetwork->getPlayerId());
    if (playerEntity.id == 0 && playerEntity.version == 0)
        return;
    auto& dash = _registry.get<Dash>(playerEntity);
    auto& velocity = _registry.get<Velocity>(playerEntity);
    auto& rigidBody = _registry.get<RigidBody>(playerEntity);

    changeDashComponentProperties(dash, velocity, rigidBody, TICK_DURATION);
    changeRigidBodyComponentProperties(rigidBody, velocity, TICK_DURATION, true);
    // GameInstancePhysics::checkCollisions(_registry, TICK_DURATION);
}

void Game::updateNetworkGameTick()
{
    auto currentInputs = getCurrentInputs();
    const auto clientId = m_clientNetwork->getPlayerId();
    const auto playerEntity = findEntityByClientId(_registry, clientId);
    float mouseX = static_cast<float>(_inputs.getMouseX());
    float mouseY = static_cast<float>(_inputs.getMouseY());
    
    // Debug: print what we're sending to server when shooting
    if (!currentInputs.empty() && _registry.has<Position>(playerEntity) && _registry.has<Hitbox>(playerEntity)) {
        const Position& playerPos = _registry.get<Position>(playerEntity);
        const Hitbox& playerHitbox = _registry.get<Hitbox>(playerEntity);
        float playerCenterX = playerPos.v.x + playerHitbox.width / 2.0f;
        float playerCenterY = playerPos.v.y + playerHitbox.height / 2.0f;
        std::cout << "CLIENT - Player center: (" << playerCenterX << ", " << playerCenterY 
                  << ") Mouse: (" << mouseX << ", " << mouseY << ")" << std::endl;
    }

    m_clientNetwork->sendCurrentInputStateWithMouse(currentInputs, mouseX, mouseY);
    m_inputHistory.recordInput(m_clientNetwork->getCurrentTick(), currentInputs);
    m_clientNetwork->incrementTick();
    if (m_clientNetwork->getCurrentTick() % 100 == 0)
        m_clientNetwork->pingRequest();
    processLocalInputs(currentInputs);
    GameInstancePhysics::updatePreviousPositions(_registry);
    updateSystemsComponents();
    movementSystem(_registry, TICK_DURATION);
    WeaponRotationSystem::updateWeaponRotations(_registry, mouseX, mouseY);
    if (_registry.has<Position>(playerEntity)) {
        const auto& pos = _registry.get<Position>(playerEntity);
        updatePlayerSprite(_registry, playerEntity, pos.v.x, pos.v.y);
    }
    spriteAnimationSystem(_registry, TICK_DURATION);
    collisionSystem(_registry, TICK_DURATION);
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

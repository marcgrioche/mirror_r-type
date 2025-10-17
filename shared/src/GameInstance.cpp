#include "../include/GameInstance.hpp"
#include "../include/game_instance/GameInstanceConditions.hpp"
#include "../include/game_instance/GameInstanceCore.hpp"
#include "../include/game_instance/GameInstanceEntities.hpp"
#include "../include/game_instance/GameInstancePhysics.hpp"
#include "../include/game_instance/GameInstancePlayer.hpp"
#include "../include/game_instance/GameInstanceSerialization.hpp"
#include "ecs/components/Velocity.hpp"
#include "ecs/systems/WeaponSystem.hpp"
#include <iostream>

GameInstance::GameInstance(uint32_t lobbyId)
    : _core(lobbyId)
    , _player()
    , _entities()
{
}

void GameInstance::initialize()
{
    _core.setLost(false);
    _core.setWon(false);
    std::cout << "Initializing game instance for lobby " << _core.getLobbyId() << std::endl;
    _entities.initializeLevel(_core.getRegistry());
    _core.setLastTickTime(std::chrono::steady_clock::now());
    _core.setRunning(true);
    std::cout << "Game instance initialized successfully" << std::endl;
}

void GameInstance::update()
{
    if (!_core.isRunning())
        return;

    auto currentTime = std::chrono::steady_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - _core.getLastTickTime()).count();
    // std::cout << "Delta time: " << deltaTime << std::endl;
    _core.setLastTickTime(currentTime);

    _accumulatedTime += std::chrono::duration<float>(deltaTime);
    while (_accumulatedTime.count() >= TICK_DURATION) {
        updateTick();
        _accumulatedTime -= std::chrono::duration<float>(TICK_DURATION);
    }
}

void GameInstance::updateTick()
{
    _core.incrementTick();

    // Update previous positions for interpolation
    GameInstancePhysics::updatePreviousPositions(_core.getRegistry());

    // Run physics systems
    int platformsToAdd = GameInstancePhysics::updateSystems(_core.getRegistry(), TICK_DURATION);
    _entities.setPlatformsToAdd(platformsToAdd);

    // Enemy weapon automatic attacks
    if (WeaponSystem::handleEnemyAttacks(_core.getRegistry(), _entities.getNewEntitiesThisTick())) {
        _core.markStateChanged();
    }

    // Check all collisions
    GameInstancePhysics::checkCollisions(_core.getRegistry(), TICK_DURATION);

    // Cleanup dead entities
    _entities.cleanupEntities(_core.getRegistry(), TICK_DURATION);

    // Check win/lose conditions
    if (GameInstanceConditions::checkLoseCondition(_core.getRegistry(), _player.getPlayerEntities())) {
        _core.setLost(true);
        _core.setRunning(false);
    } else if (GameInstanceConditions::checkWinCondition(_core.getRegistry())) {
        _core.setWon(true);
        _core.setRunning(false);
    }

    // Regenerate platforms if needed
    if (_entities.getPlatformsToAdd() > 0) {
        _entities.generatePlatforms(_core.getRegistry(), _entities.getPlatformsToAdd());
        _entities.setPlatformsToAdd(0);
    }

    // Check for velocity changes in players to mark state as changed
    for (const auto& [playerId, entity] : _player.getPlayerEntities()) {
        if (_core.getRegistry().has<Velocity>(entity)) {
            const auto& vel = _core.getRegistry().get<Velocity>(entity);
            if (vel.v.x != 0.0f || vel.v.y != 0.0f) {
                _core.markStateChanged();
                break;
            }
        }
    }
}

void GameInstance::addPlayer(uint32_t playerId, const std::string& username)
{
    _player.addPlayer(_core.getRegistry(), playerId, username, _entities.getNewEntitiesThisTick());
    _core.markStateChanged();
}

void GameInstance::removePlayer(uint32_t playerId)
{
    _player.removePlayer(_core.getRegistry(), playerId);
}

bool GameInstance::processPlayerInput(uint32_t playerId, uint32_t tick, const std::vector<std::pair<GameInput, bool>>& inputs)
{
    bool hasRealInputs = _player.processPlayerInput(_core.getRegistry(), playerId, tick, inputs, _entities.getNewEntitiesThisTick());
    if (hasRealInputs) {
        _core.markStateChanged();
    }
    return hasRealInputs;
}

std::vector<uint8_t> GameInstance::serializeGameState() const
{
    return GameInstanceSerialization::serializeGameState(
        _core.getRegistry(),
        _core.getCurrentTick(),
        _player.getPlayerEntities());
}

void GameInstance::deserializeGameState(const std::vector<uint8_t>& data)
{
    GameInstanceSerialization::deserializeGameState(data);
}

Message GameInstance::serializeEntitySpawn(Entity entity)
{
    return GameInstanceSerialization::serializeEntitySpawn(
        _core.getRegistry(),
        entity,
        _player.getPlayerEntities(),
        _player.getUsernames());
}

Message GameInstance::serializeEntityBatch(const std::vector<Entity>& entities)
{
    return GameInstanceSerialization::serializeEntityBatch(
        _core.getRegistry(),
        entities,
        _player.getPlayerEntities(),
        _player.getUsernames());
}

std::vector<Entity> GameInstance::getAndClearNewEntities()
{
    return _entities.getAndClearNewEntities();
}

std::vector<uint32_t> GameInstance::getAndClearKilledEntities()
{
    return _entities.getAndClearKilledEntities();
}

bool GameInstance::hasStateChanged()
{
    return _core.hasStateChanged();
}

uint32_t GameInstance::findPlayerIdByEntity(const Entity& entity)
{
    return _player.findPlayerIdByEntity(entity);
}

std::optional<uint32_t> GameInstance::getPlayerEntityIdById(uint32_t playerId)
{
    return _player.getPlayerEntityIdById(playerId);
}

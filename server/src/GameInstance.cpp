#include "GameInstance.hpp"
#include <iostream>

GameInstance::GameInstance(uint32_t lobbyId)
    : _lobbyId(lobbyId)
    , _isRunning(false)
    , _currentTick(0)
{
}

void GameInstance::initialize()
{
    std::cout << "Initializing game instance for lobby " << _lobbyId << std::endl;
    initializeLevel();
    _lastTickTime = std::chrono::steady_clock::now();
    _isRunning = true;
    std::cout << "Game instance initialized successfully" << std::endl;
}

void GameInstance::update()
{
    if (!_isRunning)
        return;

    auto currentTime = std::chrono::steady_clock::now();
    auto deltaTime = currentTime - _lastTickTime;
    auto tickDuration = std::chrono::duration_cast<std::chrono::steady_clock::duration>(
        std::chrono::duration<float>(TICK_DURATION));

    while (deltaTime >= tickDuration) {
        updateTick();
        deltaTime -= tickDuration;
    }

    _lastTickTime = currentTime - deltaTime;
}

void GameInstance::updateTick()
{
    _currentTick++;

    processInputs();
    simulatePhysics();
    checkCollisions();
    cleanupEntities();
}

void GameInstance::initializeLevel()
{
    // TODO: Game levels, (create platforms (same as client for now))
    factories::createOneWayPlatform(_registry, 100, 400);
    factories::createPlatform(_registry, 300, 350);
    factories::createPlatform(_registry, 500, 300);
    factories::createOneWayPlatform(_registry, 200, 250);

    for (int i = 0; i < 8; i++) {
        factories::createPlatform(_registry, i * 100, 520);
    }
}

void GameInstance::addPlayer(uint32_t playerId)
{
    std::cout << "Adding player " << playerId << " to game instance" << std::endl;
    Entity playerEntity = factories::createPlayer(_registry);
    _playerEntities[playerId] = playerEntity;

    if (_registry.has<OwnerId>(playerEntity)) {
        _registry.get<OwnerId>(playerEntity).id = playerId;
    }
}

void GameInstance::removePlayer(uint32_t playerId)
{
    auto it = _playerEntities.find(playerId);
    if (it != _playerEntities.end()) {
        _registry.kill_entity(it->second);
        _playerEntities.erase(it);
        std::cout << "Removed player " << playerId << " from game instance" << std::endl;
    }
}

void GameInstance::processPlayerInput(uint32_t playerId, uint32_t tick, const std::vector<std::pair<GameInput, bool>>& inputs)
{
    auto it = _playerEntities.find(playerId);
    if (it == _playerEntities.end())
        return;

    Entity playerEntity = it->second;

    (void)playerEntity;
    // TODO : call to systems that manage player inputs
}

void GameInstance::processInputs()
{
    // Process any buffered inputs for this tick
    // This will be called from updateTick()
}

void GameInstance::simulatePhysics()
{
    // Run shared physics systems
    gravitySystem(_registry, TICK_DURATION);
    movementSystem(_registry, TICK_DURATION);
    projectileSystem(_registry, TICK_DURATION);
}

void GameInstance::checkCollisions()
{
    collisionSystem(_registry, TICK_DURATION);
    // TODO: collisionPlayerProjectileSystem(_registry, TICK_DURATION);
    // Currently disabled due to include path issues from server directory
}

void GameInstance::cleanupEntities()
{
    auto view = _registry.view<Lifetime>();
    for (auto it = view.begin(); it != view.end();) {
        auto [lifetime] = *it;
        Entity entity = it.entity();
        lifetime.value -= TICK_DURATION;
        if (lifetime.value <= 0.0f) {
            it = view.end();
            _registry.kill_entity(entity);
            break;
        } else {
            ++it;
        }
    }
}

std::vector<uint8_t> GameInstance::serializeGameState() const
{
    std::vector<uint8_t> data;

    // Serialize tick number
    // TODO: Implement full state serialization
    // This should include positions, velocities, health, etc. for all entities

    return data;
}

void GameInstance::deserializeGameState(const std::vector<uint8_t>& data)
{
    // TODO: Implement state deserialization for rollback
}

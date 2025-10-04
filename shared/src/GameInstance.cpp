#include "../include/GameInstance.hpp"
#include "../include/Message.hpp"
#include <iostream>

#include "PlayerActions.hpp"

GameInstance::GameInstance(uint32_t lobbyId)
    : _lobbyId(lobbyId)
    , _isRunning(false)
    , _currentTick(0)
    , _stateChanged(false)
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

    for (const auto& [playerId, entity] : _playerEntities) {
        if (_registry.has<Velocity>(entity)) {
            const auto& vel = _registry.get<Velocity>(entity);
            if (vel.dx != 0.0f || vel.dy != 0.0f) {
                _stateChanged = true;
                break;
            }
        }
    }
}

void GameInstance::initializeLevel()
{
    // TODO: Game levels, (create platforms (same as client for now))
    // _newEntitiesThisTick.push_back(factories::createOneWayPlatform(_registry, 100, 400));
    // _newEntitiesThisTick.push_back(factories::createPlatform(_registry, 300, 350));
    // _newEntitiesThisTick.push_back(factories::createPlatform(_registry, 500, 300));
    // _newEntitiesThisTick.push_back(factories::createOneWayPlatform(_registry, 200, 250));
    auto platformList = factories::generateRandomPlatforms(_registry, 8);
    _newEntitiesThisTick.insert(_newEntitiesThisTick.end(), platformList.begin(), platformList.end());
    _newEntitiesThisTick.push_back(factories::createEnemy(_registry));

    // for (int i = 0; i < 8; i++) {
    //     _newEntitiesThisTick.push_back(factories::createPlatform(_registry, i * 100, 520));
    // }
}

void GameInstance::addPlayer(uint32_t playerId)
{
    std::cout << "Adding player " << playerId << " to game instance" << std::endl;
    Entity playerEntity = factories::createPlayer(_registry);
    _playerEntities[playerId] = playerEntity;
    _newEntitiesThisTick.push_back(playerEntity);
    _stateChanged = true;

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

bool GameInstance::processPlayerInput(uint32_t playerId, uint32_t tick, const std::vector<std::pair<GameInput, bool>>& inputs)
{
    // TODO : refactor
    auto it = _playerEntities.find(playerId);
    if (it == _playerEntities.end())
        return false;

    Entity playerEntity = it->second;

    const bool hasRealInputs = PlayerActions::updateVelocity(inputs, _registry, playerEntity);
    // if (!_registry.has<Velocity>(playerEntity) || !_registry.has<Jump>(playerEntity)) {
    //     return false;
    // }
    //
    // auto& velocity = _registry.get<Velocity>(playerEntity);
    // auto& jump = _registry.get<Jump>(playerEntity);
    //
    // velocity.dx = 0.0f;
    //
    // const float speed = 250.0f;
    // bool hasRealInputs = false;
    //
    // for (const auto& [input, isPressed] : inputs) {
    //     if (!isPressed)
    //         continue;
    //
    //     hasRealInputs = true;
    //     switch (input) {
    //     case GameInput::UP:
    //         if (!jump.isJumping && jump.canJump) {
    //             velocity.dy = -V0;
    //             jump.isJumping = true;
    //             jump.canJump = false;
    //         }
    //         break;
    //     case GameInput::DOWN:
    //         if (jump.isJumping && velocity.dy > 0) {
    //             velocity.dy += 300.0f; // Fast-fall
    //         }
    //         break;
    //     case GameInput::LEFT:
    //         velocity.dx = -speed;
    //         break;
    //     case GameInput::RIGHT:
    //         velocity.dx = speed;
    //         break;
    //     case GameInput::ATTACK:
    //         // TODO: Handle shooting/projectile creation
    //         break;
    //     case GameInput::DASH:
    //         // TODO: Handle dash ability
    //         break;
    //     }
    // }

    if (hasRealInputs)
        _stateChanged = true;

    return hasRealInputs;
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
    Message msg(MessageType::GAME_STATE);

    msg.write(static_cast<uint32_t>(_currentTick));

    msg.write(static_cast<uint8_t>(_playerEntities.size()));

    for (const auto& [playerId, entity] : _playerEntities) {
        msg.write(static_cast<uint32_t>(entity.id));

        if (_registry.has<Position>(entity)) {
            const auto& pos = _registry.get<Position>(entity);
            msg.write(pos.x);
            msg.write(pos.y);
        } else {
            msg.write(0.0f);
            msg.write(0.0f);
        }

        if (_registry.has<Health>(entity)) {
            const auto& health = _registry.get<Health>(entity);
            msg.write(static_cast<uint32_t>(health.hp));
        } else {
            msg.write(static_cast<uint32_t>(100));
        }
    }

    return msg.getPayload();
}

void GameInstance::deserializeGameState(const std::vector<uint8_t>& data)
{
    // TODO: Implement state deserialization for rollback
}

Message GameInstance::serializeEntitySpawn(Entity entity)
{
    uint8_t entityType = 255;

    if (_registry.has<PlayerTag>(entity)) {
        entityType = 0; // Player
    } else if (_registry.has<ProjectileTag>(entity)) {
        entityType = 1; // Projectile
    } else if (_registry.has<PlatformTag>(entity)) {
        entityType = 2; // Platform
    } else if (_registry.has<EnemyTag>(entity)) {
        entityType = 3; // Enemy
    }

    if (entityType == 255) {
        return Message(MessageType::SPAWN_ENTITY);
    }

    Message msg(MessageType::SPAWN_ENTITY);
    msg.write(static_cast<uint32_t>(entity.id));
    msg.write(entityType);

    if (_registry.has<Position>(entity)) {
        auto& pos = _registry.get<Position>(entity);
        msg.write(pos.x);
        msg.write(pos.y);
    } else {
        return Message(MessageType::SPAWN_ENTITY);
    }

    if (entityType == 0) { // Player
        if (_registry.has<Health>(entity)) {
            auto& health = _registry.get<Health>(entity);
            msg.write(static_cast<uint32_t>(health.hp));
        }

        if (_registry.has<Hitbox>(entity)) {
            auto& hitbox = _registry.get<Hitbox>(entity);
            msg.write(hitbox.width);
            msg.write(hitbox.height);
            msg.write(hitbox.offset_x);
            msg.write(hitbox.offset_y);
        }

        if (_registry.has<OwnerId>(entity)) {
            auto& owner = _registry.get<OwnerId>(entity);
            msg.write(static_cast<uint32_t>(owner.id));
        } else {
            msg.write(static_cast<uint32_t>(0)); // Default player ID
        }

    } else if (entityType == 1) { // Projectile
        if (_registry.has<Velocity>(entity)) {
            auto& vel = _registry.get<Velocity>(entity);
            msg.write(vel.dx);
            msg.write(vel.dy);
        }

        if (_registry.has<Damage>(entity)) {
            auto& damage = _registry.get<Damage>(entity);
            msg.write(damage.value);
        }

        if (_registry.has<Hitbox>(entity)) {
            auto& hitbox = _registry.get<Hitbox>(entity);
            msg.write(hitbox.width);
            msg.write(hitbox.height);
            msg.write(hitbox.offset_x);
            msg.write(hitbox.offset_y);
        }

        if (_registry.has<OwnerId>(entity)) {
            auto& owner = _registry.get<OwnerId>(entity);
            msg.write(static_cast<uint32_t>(owner.id));
        }

        if (_registry.has<Lifetime>(entity)) {
            auto& lifetime = _registry.get<Lifetime>(entity);
            msg.write(lifetime.value);
        }

    } else if (entityType == 2) { // Platform
        if (_registry.has<Hitbox>(entity)) {
            auto& hitbox = _registry.get<Hitbox>(entity);
            msg.write(hitbox.width);
            msg.write(hitbox.height);
            msg.write(hitbox.offset_x);
            msg.write(hitbox.offset_y);
        }

    } else if (entityType == 3) { // Enemy
        if (_registry.has<Health>(entity)) {
            auto& health = _registry.get<Health>(entity);
            msg.write(static_cast<uint32_t>(health.hp));
        }

        if (_registry.has<Hitbox>(entity)) {
            auto& hitbox = _registry.get<Hitbox>(entity);
            msg.write(hitbox.width);
            msg.write(hitbox.height);
            msg.write(hitbox.offset_x);
            msg.write(hitbox.offset_y);
        }
    }

    return msg;
}

std::vector<Entity> GameInstance::getAndClearNewEntities()
{
    std::vector<Entity> entities = std::move(_newEntitiesThisTick);
    _newEntitiesThisTick.clear();
    return entities;
}

bool GameInstance::hasStateChanged()
{
    bool changed = _stateChanged;
    _stateChanged = false;
    return changed;
}

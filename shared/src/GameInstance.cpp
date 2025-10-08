#include "../include/GameInstance.hpp"
#include "../include/Message.hpp"
#include "Parent.hpp"
#include "ecs/components/PreviousPosition.hpp"
#include "ecs/components/Dash.hpp"
#include "ecs/components/Dead.hpp"
#include "ecs/components/Jump.hpp"
#include "ecs/components/Velocity.hpp"
#include "ecs/systems/ColisionPlayerPowerUpSystem.hpp"
#include "ecs/systems/DashSystem.hpp"
#include "ecs/systems/FrequencyUtils.hpp"
#include "ecs/systems/MovementSystem.hpp"
#include "ecs/systems/PowerUpEffectSystem.hpp"
#include "ecs/systems/PowerUpSystem.hpp"
#include "ecs/systems/WeaponSystem.hpp"
#include "entities/enemies/EnemyMovement.hpp"
#include "entities/powerUp/CreatePowerUp.hpp"
#include <cstdlib>
#include <iostream>
#include <unordered_set>

GameInstance::GameInstance(uint32_t lobbyId)
    : _lobbyId(lobbyId)
    , _isRunning(false)
    , _currentTick(0)
    , _stateChanged(false)
    , _gameWon(false)
    , _gameLost(false)
{
}

void GameInstance::initialize()
{
    std::cout << "Initializing game instance for lobby " << _lobbyId << std::endl;
    initializeLevel();
    _lastTickTime = std::chrono::steady_clock::now();
    _lastPowerUpSpawnTime = std::chrono::steady_clock::now() - std::chrono::seconds(static_cast<long long>(POWER_UP_SPAWN_INTERVAL));
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

bool GameInstance::checkLoseCondition() const
{
    for (const auto& [playerId, entity] : _playerEntities) {
        if (_registry.has<Health>(entity) && _registry.get<Health>(entity).hp > 0) {
            return false;
        }
    }
    return true;
}

bool GameInstance::checkWinCondition() const
{
    // TODO: Implement win condition
    return false;
}

void GameInstance::updateTick()
{
    _currentTick++;

    auto prevPosView = _registry.view<Position, PreviousPosition>();
    for (auto&& [pos, prevPos] : prevPosView) {
        prevPos.x = pos.x;
        prevPos.y = pos.y;
    }

    processInputs();
    dashSystem(_registry, TICK_DURATION);
    enemyMovement(_registry, TICK_DURATION);
    gravitySystem(_registry, TICK_DURATION);

    _platformsToAdd = movementSystem(_registry, TICK_DURATION);
    boundarySystem(_registry);

    // Enemy weapon automatic attacks
    if (WeaponSystem::handleEnemyAttacks(_registry, _newEntitiesThisTick)) {
        _stateChanged = true;
    }

    checkCollisions();
    powerUpEffectSystem(_registry, TICK_DURATION);

    cleanupEntities();
    powerUpSystem(_registry, TICK_DURATION);

    if (checkLoseCondition()) {
        _gameLost = true;
        _isRunning = false;
    } else if (checkWinCondition()) {
        _gameWon = true;
        _isRunning = false;
    }

    if (_platformsToAdd > 0) {
        for (; _platformsToAdd > 0; _platformsToAdd--) {
            auto platformTmp = factories::reGenerateRandomPlatforms(_registry, 1);
            _newEntitiesThisTick.insert(_newEntitiesThisTick.end(), platformTmp.begin(), platformTmp.end());
            std::cout << _platformsToAdd << std::endl;
        }
    }

    auto currentTime = std::chrono::steady_clock::now();
    auto timeSinceLastSpawn = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - _lastPowerUpSpawnTime);

    // It spwans random powerups on random coords, useless
    // if (timeSinceLastSpawn.count() >= POWER_UP_SPAWN_INTERVAL && !_playerEntities.empty()) {
    //     spawnRandomPowerUps(static_cast<int>(_playerEntities.size()));
    //     _lastPowerUpSpawnTime = currentTime;
    // }

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
    auto platformList = factories::generateRandomPlatforms(_registry, 8);
    _newEntitiesThisTick.insert(_newEntitiesThisTick.end(), platformList.begin(), platformList.end());
    _newEntitiesThisTick.push_back(factories::createEnemy(_registry, Position {700, 100}, Health {15}, Hitbox {32, 32}));
    _newEntitiesThisTick.push_back(factories::createEnemy(_registry, Position {700, 200}, Health {15}, Hitbox {32, 32}));
    _newEntitiesThisTick.push_back(factories::createEnemy(_registry, Position {700, 300}, Health {15}, Hitbox {32, 32}));
    _newEntitiesThisTick.push_back(factories::createEnemy(_registry, Position {700, 400}, Health {15}, Hitbox {32, 32}));
    _newEntitiesThisTick.push_back(factories::createEnemy(_registry, Position {700, 500}, Health {15}, Hitbox {32, 32}));
}

void GameInstance::addPlayer(uint32_t playerId)
{
    std::cout << "Adding player " << playerId << " to game instance" << std::endl;
    Entity playerEntity = factories::createPlayer(_registry);
    _playerEntities[playerId] = playerEntity;
    _newEntitiesThisTick.push_back(playerEntity);
    _stateChanged = true;
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

    if (!_registry.has<Velocity>(playerEntity) || !_registry.has<Jump>(playerEntity) || !_registry.has<Dash>(playerEntity)) {
        return false;
    }

    auto& velocity = _registry.get<Velocity>(playerEntity);
    auto& jump = _registry.get<Jump>(playerEntity);
    auto& dash = _registry.get<Dash>(playerEntity);

    if (!dash.isDashing) {
        velocity.dx = 0.0f;
    }

    const float speed = 250.0f;
    bool hasRealInputs = false;

    for (const auto& [input, isPressed] : inputs) {
        if (!isPressed)
            continue;

        hasRealInputs = true;
        switch (input) {
        case GameInput::UP:
            if (!jump.isJumping && jump.canJump) {
                velocity.dy = -V0;
                jump.isJumping = true;
                jump.canJump = false;
            }
            break;
        case GameInput::DOWN:
            if (jump.isJumping && velocity.dy > 0) {
                velocity.dy += 300.0f; // Fast-fall
            }
            break;
        case GameInput::LEFT:
            if (!dash.isDashing) {
                velocity.dx = -speed;
                dash.direction = -1;
            }
            break;
        case GameInput::RIGHT:
            if (!dash.isDashing) {
                velocity.dx = speed;
                dash.direction = 1;
            }
            break;
        case GameInput::ATTACK:
            if (WeaponSystem::handlePlayerAttack(_registry, playerEntity, playerId, _newEntitiesThisTick)) {
                _stateChanged = true;
            }
            break;
        case GameInput::DASH:
            if (!dash.isDashing && FrequencyUtils::shouldTrigger(dash.cooldown)) {
                dash.isDashing = true;
                dash.remaining = dash.duration;
                if (velocity.dx < 0) {
                    dash.direction = -1;
                } else if (velocity.dx > 0) {
                    dash.direction = 1;
                }
                _stateChanged = true;
            }
            break;
        }
    }

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
    enemyMovement(_registry, TICK_DURATION);
    gravitySystem(_registry, TICK_DURATION);
    movementSystem(_registry, TICK_DURATION);
    projectileSystem(_registry, TICK_DURATION);
}

void GameInstance::checkCollisions()
{
    collisionSystem(_registry, TICK_DURATION);
    collisionPlayerProjectileSystem(_registry, TICK_DURATION);
    collisionEnemyProjectileSystem(_registry, TICK_DURATION);
    collisionPlayerPowerUpSystem(_registry, TICK_DURATION);
}

void GameInstance::cleanupEntities()
{
    // 1) Handle lifetime expiration (collect all that reach 0 this tick)
    std::vector<Entity> killedThisTick;
    {
        auto view = _registry.view<Lifetime>();
        for (auto it = view.begin(); it != view.end(); ++it) {
            auto [lifetime] = *it;
            Entity entity = it.entity();
            lifetime.value -= TICK_DURATION; // mutate component
            if (lifetime.value <= 0.0f) {
                killedThisTick.push_back(entity);
            }
        }
    }

    // 2) Collect entities flagged Dead
    {
        auto viewDead = _registry.view<Dead>();
        for (auto it = viewDead.begin(); it != viewDead.end(); ++it) {
            auto [dead] = *it;
            if (dead.dead == true) {
                killedThisTick.push_back(it.entity());
            }
        }
    }

    if (killedThisTick.empty())
        return; // nothing to do

    // 3) Record killed entity IDs (deduplicated) then physically remove from registry
    {
        std::unordered_set<uint32_t> already; // dedupe by raw id for network despawn
        for (auto e : killedThisTick) {
            if (already.insert(e.id).second) {
                _killedEntitiesThisTick.push_back(e.id);
            }
            if (_registry.has<EnemyTag>(e) && rand() % 3 == 0) {
                Position& pos = _registry.get<Position>(e);
                PowerUpType type = (rand() % 2 == 0)
                    ? PowerUpType::HEAL
                    : PowerUpType::DAMAGE_BOOST;
                float effectDuration = (type == PowerUpType::DAMAGE_BOOST) ? 10.0f : 0.0f;
                _newEntitiesThisTick.push_back(factories::createPowerUp(
                    _registry,
                    Position{pos.x, pos.y},
                    Velocity{-10.0f, 0.0f},
                    Hitbox{POWERUP_WIDTH, POWERUP_HEIGHT},
                    Lifetime{POWERUP_LIFETIME},
                    type,
                    effectDuration
                ));
            }
            _registry.kill_entity(e);
        }
    }

    // 4) Purge references in _playerEntities (so they "disappear completely")
    //    Also remove any player entity whose version no longer matches (already killed) just in case.
    for (auto it = _playerEntities.begin(); it != _playerEntities.end();) {
        Entity e = it->second;
        bool wasKilled = false;
        for (auto const& k : killedThisTick) {
            if (k == e) { // uses id+version compare
                wasKilled = true;
                break;
            }
        }
        // If entity id reused after kill, version mismatch will make k==e false; also treat lack of any component as invalid.
        if (wasKilled || (!_registry.has<Health>(e) && !_registry.has<Position>(e))) {
            it = _playerEntities.erase(it);
            _stateChanged = true;
        } else {
            ++it;
        }
    }

    // 5) Remove from newly spawned list if they died the same tick (avoid ghost spawn messages)
    if (!_newEntitiesThisTick.empty()) {
        _newEntitiesThisTick.erase(std::remove_if(_newEntitiesThisTick.begin(), _newEntitiesThisTick.end(), [&](Entity const& cand) {
            for (auto const& k : killedThisTick) {
                if (cand == k)
                    return true;
            }
            return false;
        }), _newEntitiesThisTick.end());
    }

    // Flag state change if anything died
    if (!killedThisTick.empty()) {
        _stateChanged = true;
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
    } else if (_registry.has<PowerUpTag>(entity)) {
        entityType = 4; // Power-up
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

        msg.write(static_cast<uint32_t>(entity.id)); // compability with server ?

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

        // Send parent entity ID and version
        if (_registry.has<Parent>(entity)) {
            auto& parent = _registry.get<Parent>(entity);
            msg.write(static_cast<uint32_t>(parent.parent.id));
            msg.write(static_cast<uint32_t>(parent.parent.version));
        } else {
            msg.write(static_cast<uint32_t>(0));
            msg.write(static_cast<uint32_t>(0));
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
        if (_registry.has<Velocity>(entity)) {
            auto& vel = _registry.get<Velocity>(entity);
            msg.write(vel.dx);
            msg.write(vel.dy);
        }

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
    } else if (entityType == 4) { // Power-up
        if (_registry.has<PowerUp>(entity)) {
            auto& powerUp = _registry.get<PowerUp>(entity);
            msg.write(static_cast<uint8_t>(powerUp.type));
            msg.write(powerUp.effect_duration);
        }

        if (_registry.has<Hitbox>(entity)) {
            auto& hitbox = _registry.get<Hitbox>(entity);
            msg.write(hitbox.width);
            msg.write(hitbox.height);
            msg.write(hitbox.offset_x);
            msg.write(hitbox.offset_y);
        }

        if (_registry.has<Lifetime>(entity)) {
            auto& lifetime = _registry.get<Lifetime>(entity);
            msg.write(lifetime.value);
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

std::vector<uint32_t> GameInstance::getAndClearKilledEntities()
{
    std::vector<uint32_t> entities = std::move(_killedEntitiesThisTick);
    _killedEntitiesThisTick.clear();
    return entities;
}

bool GameInstance::hasStateChanged()
{
    bool changed = _stateChanged;
    _stateChanged = false;
    return changed;
}

void GameInstance::spawnRandomPowerUps(int count)
{
    for (int i = 0; i < count; ++i) {
        // Random position on screen (adjust based on your screen dimensions)
        float x = static_cast<float>(rand() % 800 + 100); // Random X between 100-900
        float y = static_cast<float>(rand() % 300 + 200); // Random Y between 200-500

        // Random power-up type
        PowerUpType type = (rand() % 2 == 0) ? PowerUpType::HEAL : PowerUpType::DAMAGE_BOOST;

        // Effect duration: 10 seconds for damage boost, instant for heal
        float effectDuration = (type == PowerUpType::DAMAGE_BOOST) ? 10.0f : 0.0f;

        Position pos = { x, y };
        Velocity vel = { 0.0f, 0.0f }; // Stationary power-ups
        Hitbox hitbox = { 20.0f, 20.0f, 0.0f, 0.0f }; // 20x20 hitbox
        Lifetime lifetime = { 30.0f }; // 30 seconds lifetime

        Entity powerUp = factories::createPowerUp(_registry, pos, vel, hitbox, lifetime, type, effectDuration);
        _newEntitiesThisTick.push_back(powerUp);
    }
}

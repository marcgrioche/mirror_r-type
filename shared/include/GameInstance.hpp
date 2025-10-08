#pragma once

#include "../../shared/include/GameInputs.hpp"
#include "../../shared/include/Message.hpp"
#include "../../shared/src/ecs/Registry.hpp"
#include "../../shared/src/ecs/components/Position.hpp"
#include "../../shared/src/ecs/components/Velocity.hpp"
#include "../../shared/src/ecs/components/Health.hpp"
#include "../../shared/src/ecs/components/Hitbox.hpp"
#include "../../shared/src/ecs/components/Player.hpp"
#include "../../shared/src/ecs/components/Damage.hpp"
#include "../../shared/src/ecs/components/Lifetime.hpp"
#include "../../shared/src/ecs/components/PowerUp.hpp"
#include "../../shared/src/ecs/components/Tags.hpp"
#include "../../shared/src/ecs/components/Projectile.hpp"
#include "../../shared/src/ecs/systems/BoundarySystem.hpp"
#include "../../shared/src/ecs/systems/CollisionSystem.hpp"
#include "../../shared/src/ecs/systems/CollisionPlayerProjectileSystem.hpp"
#include "../../shared/src/ecs/systems/CollisionEnemyProjectileSystem.hpp"
#include "../../shared/src/ecs/systems/GravitySystem.hpp"
#include "../../shared/src/ecs/systems/MovementSystem.hpp"
#include "../../shared/src/ecs/systems/ProjectileSystem.hpp"
#include "../../shared/src/entities/enemies/CreateEnemy.hpp"
#include "../../shared/src/entities/platform/CreatePlatform.hpp"
#include "../../shared/src/entities/player/CreatePlayer.hpp"
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
#include "entities/enemies/CreateEnemy.hpp"
#include "entities/powerUp/CreatePowerUp.hpp"
#include <cstdlib>
#include <iostream>
#include <unordered_set>
#include <chrono>
#include <cstdint>
#include <functional>
#include <unordered_map>
#include <vector>

class GameInstance {
public:
    static constexpr uint32_t TICKS_PER_SECOND = 60;
    static constexpr float TICK_DURATION = 1.0f / TICKS_PER_SECOND;

    GameInstance(uint32_t lobbyId);
    ~GameInstance() = default;

    void initialize();
    void update();
    bool isRunning() const { return _isRunning; }
    void stop() { _isRunning = false; }

    bool hasWon() const { return _gameWon; }
    bool hasLost() const { return _gameLost; }

    uint32_t getLobbyId() const { return _lobbyId; }
    uint32_t getCurrentTick() const { return _currentTick; }

    void addPlayer(uint32_t playerId);
    void removePlayer(uint32_t playerId);
    bool processPlayerInput(uint32_t playerId, uint32_t tick, const std::vector<std::pair<GameInput, bool>>& inputs);

    std::vector<uint8_t> serializeGameState() const;
    void deserializeGameState(const std::vector<uint8_t>& data);

    /**
     * Serialize entity spawn data for SPAWN_ENTITY message.
     *
     * Args:
     *     entity (Entity): The entity to serialize
     *
     * Returns:
     *     std::vector<uint8_t>: Serialized spawn data, or empty if entity type not supported
     */
    Message serializeEntitySpawn(Entity entity);

    /**
     * Get newly spawned entities from this tick and clear the list.
     *
     * Returns:
     *     std::vector<Entity>: Entities created this tick
     */
    std::vector<Entity> getAndClearNewEntities();

    /**
     * Get entities killed this tick and clear the list.
     *
     * Returns:
     *     std::vector<uint32_t>: Entity IDs killed this tick
     */
    std::vector<uint32_t> getAndClearKilledEntities();

    /**
     * Check if game state changed this tick and reset the flag.
     *
     * Returns:
     *     bool: True if state changed since last check
     */
    bool hasStateChanged();

    /**
     * Get registry for client prediction access.
     *
     * Returns:
     *     Registry&: Reference to the game registry
     */
    Registry& getRegistry() { return _registry; }
    const Registry& getRegistry() const { return _registry; }

private:
    uint32_t _lobbyId;
    Registry _registry;
    bool _isRunning;
    uint32_t _currentTick;
    std::chrono::steady_clock::time_point _lastTickTime;
    bool _stateChanged;

    std::unordered_map<uint32_t, Entity> _playerEntities;
    std::vector<Entity> _newEntitiesThisTick;
    std::vector<uint32_t> _killedEntitiesThisTick;

    bool _gameWon;
    bool _gameLost;

    int _platformsToAdd = 0;

    std::chrono::steady_clock::time_point _lastPowerUpSpawnTime;
    static constexpr float POWER_UP_SPAWN_INTERVAL = 30.0f;

    void updateTick();
    void initializeLevel();
    void processInputs();
    void simulatePhysics();
    void checkCollisions();
    void cleanupEntities();
    void spawnRandomPowerUps(int count);

    bool checkLoseCondition() const;
    bool checkWinCondition() const;
};

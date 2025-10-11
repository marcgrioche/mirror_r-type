#pragma once

#include "../../shared/include/GameInputs.hpp"
#include "../../shared/include/Message.hpp"
#include "../../shared/src/ecs/Registry.hpp"
#include "../../shared/src/ecs/components/Damage.hpp"
#include "../../shared/src/ecs/components/Health.hpp"
#include "../../shared/src/ecs/components/Hitbox.hpp"
#include "../../shared/src/ecs/components/Lifetime.hpp"
#include "../../shared/src/ecs/components/Player.hpp"
#include "../../shared/src/ecs/components/Position.hpp"
#include "../../shared/src/ecs/components/PowerUp.hpp"
#include "../../shared/src/ecs/components/Projectile.hpp"
#include "../../shared/src/ecs/components/Tags.hpp"
#include "../../shared/src/ecs/components/Velocity.hpp"
#include "../../shared/src/ecs/systems/BoundarySystem.hpp"
#include "../../shared/src/ecs/systems/CollisionEnemyProjectileSystem.hpp"
#include "../../shared/src/ecs/systems/CollisionPlayerProjectileSystem.hpp"
#include "../../shared/src/ecs/systems/CollisionSystem.hpp"
#include "../../shared/src/ecs/systems/GravitySystem.hpp"
#include "../../shared/src/ecs/systems/MovementSystem.hpp"
#include "../../shared/src/ecs/systems/ProjectileSystem.hpp"
#include "../../shared/src/entities/enemies/CreateEnemy.hpp"
#include "../../shared/src/entities/platform/CreatePlatform.hpp"
#include "../../shared/src/entities/player/CreatePlayer.hpp"
#include "../include/Message.hpp"
#include "Config.hpp"
#include "Parent.hpp"
#include "components/RigidBody.hpp"
#include "ecs/components/Dash.hpp"
#include "ecs/components/Dead.hpp"
#include "ecs/components/PreviousPosition.hpp"
#include "ecs/components/Velocity.hpp"
#include "ecs/systems/ColisionPlayerPowerUpSystem.hpp"
#include "ecs/systems/DashSystem.hpp"
#include "ecs/systems/FrequencyUtils.hpp"
#include "ecs/systems/MovementSystem.hpp"
#include "ecs/systems/PowerUpEffectSystem.hpp"
#include "ecs/systems/PowerUpSystem.hpp"
#include "ecs/systems/WeaponSystem.hpp"
#include "entities/boss/CreateBoss.hpp"
#include "entities/enemies/CreateEnemy.hpp"
#include "entities/powerUp/CreatePowerUp.hpp"
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class GameInstance {
public:
    static constexpr uint32_t TICKS_PER_SECOND = 60;
    static constexpr float TICK_DURATION = 1.0f / TICKS_PER_SECOND;

    /**
     * @brief Constructs a GameInstance for the specified lobby
     * @param lobbyId The ID of the lobby this game instance belongs to
     */
    GameInstance(uint32_t lobbyId);

    /**
     * @brief Destroys the GameInstance
     */
    ~GameInstance() = default;

    /**
     * @brief Initializes the game instance and sets up the level
     */
    void initialize();

    /**
     * @brief Updates the game state for one tick
     */
    void update();

    /**
     * @brief Checks if the game instance is currently running
     * @return True if the game is running, false otherwise
     */
    bool isRunning() const { return _isRunning; }

    /**
     * @brief Stops the game instance
     */
    void stop() { _isRunning = false; }

    /**
     * @brief Checks if the game has been won
     * @return True if the game has been won, false otherwise
     */
    bool hasWon() const { return _gameWon; }

    /**
     * @brief Checks if the game has been lost
     * @return True if the game has been lost, false otherwise
     */
    bool hasLost() const { return _gameLost; }

    /**
     * @brief Gets the lobby ID this game instance belongs to
     * @return The lobby ID
     */
    uint32_t getLobbyId() const { return _lobbyId; }

    /**
     * @brief Gets the current game tick number
     * @return The current tick number
     */
    uint32_t getCurrentTick() const { return _currentTick; }

    /**
     * @brief Adds a player to the game instance
     * @param playerId The player's unique ID
     * @param username The player's username
     */
    void addPlayer(uint32_t playerId, const std::string& username);

    /**
     * @brief Removes a player from the game instance
     * @param playerId The player's unique ID to remove
     */
    void removePlayer(uint32_t playerId);

    /**
     * @brief Processes input from a player
     * @param playerId The player's unique ID
     * @param tick The tick number this input corresponds to
     * @param inputs Vector of input pairs (GameInput, isPressed)
     * @return True if input was processed successfully, false otherwise
     */
    bool processPlayerInput(uint32_t playerId, uint32_t tick, const std::vector<std::pair<GameInput, bool>>& inputs);

    /**
     * @brief Serializes the current game state
     * @return Serialized game state as a byte vector
     */
    std::vector<uint8_t> serializeGameState() const;

    /**
     * @brief Deserializes and applies a game state
     * @param data Serialized game state data
     */
    void deserializeGameState(const std::vector<uint8_t>& data);

    /**
     * @brief Finds a player ID by their entity
     * @param t_entity The entity to search for
     * @return The player ID, or 0 if not found
     */
    uint32_t findPlayerIdByEntity(const Entity& t_entity);

    /**
     * @brief Serializes entity spawn data for SPAWN_ENTITY message
     * @param entity The entity to serialize
     * @return Serialized spawn data, or empty if entity type not supported
     */
    Message serializeEntitySpawn(Entity entity);

    /**
     * @brief Serializes multiple entities spawn data for SPAWN_ENTITY message
     * @param entities The entities to serialize
     * @return Serialized spawn message with count prefix
     */
    Message serializeEntityBatch(const std::vector<Entity>& entities);

    /**
     * @brief Gets newly spawned entities from this tick and clears the list
     * @return Entities created this tick
     */
    std::vector<Entity> getAndClearNewEntities();

    /**
     * @brief Gets entities killed this tick and clears the list
     * @return Entity IDs killed this tick
     */
    std::vector<uint32_t> getAndClearKilledEntities();

    /**
     * @brief Checks if game state changed this tick and resets the flag
     * @return True if state changed since last check
     */
    bool hasStateChanged();

    /**
     * @brief Gets the registry for client prediction access
     * @return Reference to the game registry
     */
    Registry& getRegistry() { return _registry; }

    /**
     * @brief Gets the registry for read-only access
     * @return Const reference to the game registry
     */
    const Registry& getRegistry() const { return _registry; }

    /**
     * @brief Gets a player's entity ID by their player ID
     * @param playerId The player's unique ID
     * @return Optional containing the entity ID if found
     */
    std::optional<uint32_t> getPlayerEntityIdById(uint32_t playerId);

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
    std::unordered_map<uint32_t, std::string> _usernames;

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

#pragma once

#include "GameInputs.hpp"
#include "Message.hpp"
#include "game_instance/GameInstanceConditions.hpp"
#include "game_instance/GameInstanceCore.hpp"
#include "game_instance/GameInstanceEntities.hpp"
#include "game_instance/GameInstancePhysics.hpp"
#include "game_instance/GameInstancePlayer.hpp"
#include "game_instance/GameInstanceSerialization.hpp"
#include "levels/Level.hpp"
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

/**
 * @brief Main game instance orchestrating all game subsystems
 *
 * This class delegates responsibilities to specialized managers:
 * - Core: State and lifecycle management
 * - Player: Player management and input processing
 * - Entities: Entity lifecycle and spawning
 * - Physics: Physics systems and collisions
 * - Conditions: Win/lose condition checks
 * - Serialization: State and entity serialization
 */
class GameInstance {
public:
    static constexpr uint32_t TICKS_PER_SECOND = 60;
    static constexpr float TICK_DURATION = 1.0f / TICKS_PER_SECOND;

    /**
     * @brief Constructs a GameInstance for the specified lobby
     * @param lobbyId The ID of the lobby this game instance belongs to
     */
    explicit GameInstance(uint32_t lobbyId);

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
     */
    bool isRunning() const { return _core.isRunning(); }

    /**
     * @brief Stops the game instance
     */
    void stop() { _core.setRunning(false); }

    /**
     * @brief Checks if the game has been won
     */
    bool hasWon() const { return _core.hasWon(); }

    /**
     * @brief Checks if the game has been lost
     */
    bool hasLost() const { return _core.hasLost(); }

    /**
     * @brief Gets the lobby ID this game instance belongs to
     */
    uint32_t getLobbyId() const { return _core.getLobbyId(); }

    /**
     * @brief Gets the current game tick number
     */
    uint32_t getCurrentTick() const { return _core.getCurrentTick(); }

    /**
     * @brief Adds a player to the game instance
     */
    void addPlayer(uint32_t playerId, const std::string& username);

    /**
     * @brief Removes a player from the game instance
     */
    void removePlayer(uint32_t playerId);

    /**
     * @brief Processes input from a player
     */
    bool processPlayerInput(uint32_t playerId, uint32_t tick, const std::vector<std::pair<GameInput, bool>>& inputs);

    /**
     * @brief Serializes the current game state
     */
    std::vector<uint8_t> serializeGameState() const;

    /**
     * @brief Deserializes and applies a game state
     */
    void deserializeGameState(const std::vector<uint8_t>& data);

    /**
     * @brief Finds a player ID by their entity
     */
    uint32_t findPlayerIdByEntity(const Entity& entity);

    /**
     * @brief Serializes entity spawn data
     */
    Message serializeEntitySpawn(Entity entity);

    /**
     * @brief Serializes multiple entities spawn data
     */
    Message serializeEntityBatch(const std::vector<Entity>& entities);

    /**
     * @brief Gets newly spawned entities and clears the list
     */
    std::vector<Entity> getAndClearNewEntities();

    /**
     * @brief Gets killed entity IDs and clears the list
     */
    std::vector<uint32_t> getAndClearKilledEntities();

    /**
     * @brief Checks if game state changed this tick
     */
    bool hasStateChanged();

    /**
     * @brief Gets the registry for client prediction access
     */
    Registry& getRegistry() { return _core.getRegistry(); }

    /**
     * @brief Gets the registry for read-only access
     */
    const Registry& getRegistry() const { return _core.getRegistry(); }

    /**
     * @brief Gets a player's entity ID by their player ID
     */
    std::optional<uint32_t> getPlayerEntityIdById(uint32_t playerId);

private:
    GameInstanceCore _core;
    GameInstancePlayer _player;
    GameInstanceEntities _entities;
    Level _level;

    void updateTick();
};

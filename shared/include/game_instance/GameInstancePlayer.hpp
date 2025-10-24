#pragma once

#include "../GameInputs.hpp"
#include "../../src/ecs/Registry.hpp"
#include "../../src/ecs/Entity.hpp"
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Manages player entities and their inputs
 */
class GameInstancePlayer {
public:
    /**
     * @brief Constructs player manager
     */
    GameInstancePlayer() = default;

    /**
     * @brief Adds a player to the game
     * @param registry The game registry
     * @param playerId The player's unique ID
     * @param username The player's username
     * @param newEntities List to add the new entity to
     */
    void addPlayer(Registry& registry, uint32_t playerId, const std::string& username, 
                   std::vector<Entity>& newEntities);

    /**
     * @brief Removes a player from the game
     * @param registry The game registry
     * @param playerId The player's unique ID
     */
    void removePlayer(Registry& registry, uint32_t playerId);

    /**
     * @brief Processes input from a player
     * @param registry The game registry
     * @param playerId The player's unique ID
     * @param tick The tick number (unused currently)
     * @param inputs Vector of input pairs
     * @param newEntities List to add new entities to
     * @param mouseX Mouse X coordinate (optional, default 0)
     * @param mouseY Mouse Y coordinate (optional, default 0)
     * @return True if valid input was processed
     */
    bool processPlayerInput(Registry& registry, uint32_t playerId, uint32_t tick,
                           const std::vector<std::pair<GameInput, bool>>& inputs,
                           std::vector<Entity>& newEntities,
                           float mouseX = 0.0f, float mouseY = 0.0f);

    /**
     * @brief Finds a player ID by their entity
     * @param entity The entity to search for
     * @return The player ID, or 0 if not found
     */
    uint32_t findPlayerIdByEntity(const Entity& entity) const;

    /**
     * @brief Gets a player's entity ID by their player ID
     * @param playerId The player's unique ID
     * @return Optional containing the entity ID if found
     */
    std::optional<uint32_t> getPlayerEntityIdById(uint32_t playerId) const;

    /**
     * @brief Gets the player entities map
     */
    const std::unordered_map<uint32_t, Entity>& getPlayerEntities() const { return _playerEntities; }

    /**
     * @brief Gets the usernames map
     */
    const std::unordered_map<uint32_t, std::string>& getUsernames() const { return _usernames; }

    /**
     * @brief Cleans up dead player entities
     * @param registry The game registry
     * @param killedEntities List of killed entities this tick
     * @return True if any players were removed
     */
    bool cleanupDeadPlayers(Registry& registry, const std::vector<Entity>& killedEntities);

private:
    std::unordered_map<uint32_t, Entity> _playerEntities;
    std::unordered_map<uint32_t, std::string> _usernames;
};

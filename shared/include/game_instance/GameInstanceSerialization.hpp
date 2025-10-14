#pragma once

#include "../Message.hpp"
#include "../../src/ecs/Registry.hpp"
#include "../../src/ecs/Entity.hpp"
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <string>

/**
 * @brief Handles serialization and deserialization of game state and entities
 */
class GameInstanceSerialization {
public:
    /**
     * @brief Serializes the current game state
     * @param registry The game registry
     * @param currentTick Current tick number
     * @param playerEntities Map of player IDs to entities
     * @return Serialized game state
     */
    static std::vector<uint8_t> serializeGameState(
        const Registry& registry,
        uint32_t currentTick,
        const std::unordered_map<uint32_t, Entity>& playerEntities
    );

    /**
     * @brief Deserializes game state (TODO: implement for rollback)
     * @param data Serialized state data
     */
    static void deserializeGameState(const std::vector<uint8_t>& data);

    /**
     * @brief Serializes a single entity spawn
     * @param registry The game registry
     * @param entity The entity to serialize
     * @param playerEntities Map of player IDs to entities
     * @param usernames Map of entity IDs to usernames
     * @return Serialized spawn message
     */
    static Message serializeEntitySpawn(
        const Registry& registry,
        Entity entity,
        const std::unordered_map<uint32_t, Entity>& playerEntities,
        const std::unordered_map<uint32_t, std::string>& usernames
    );

    /**
     * @brief Serializes multiple entity spawns
     * @param registry The game registry
     * @param entities The entities to serialize
     * @param playerEntities Map of player IDs to entities
     * @param usernames Map of entity IDs to usernames
     * @return Serialized batch spawn message
     */
    static Message serializeEntityBatch(
        const Registry& registry,
        const std::vector<Entity>& entities,
        const std::unordered_map<uint32_t, Entity>& playerEntities,
        const std::unordered_map<uint32_t, std::string>& usernames
    );

private:
    /**
     * @brief Helper to find player ID by entity
     */
    static uint32_t findPlayerIdByEntity(
        const Entity& entity,
        const std::unordered_map<uint32_t, Entity>& playerEntities
    );
};

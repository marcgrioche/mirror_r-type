#pragma once

#include "../../src/ecs/Registry.hpp"
#include <cstdint>

/**
 * @brief Checks win and lose conditions
 */
class GameInstanceConditions {
public:
    /**
     * @brief Checks if all players are dead (lose condition)
     * @param registry The game registry
     * @param playerEntities Map of player IDs to entities
     * @return True if all players are dead
     */
    static bool checkLoseCondition(const Registry& registry, 
                                   const std::unordered_map<uint32_t, Entity>& playerEntities);

    /**
     * @brief Checks if all bosses are defeated (win condition)
     * @param registry The game registry
     * @return True if all bosses are defeated
     */
    static bool checkWinCondition(const Registry& registry);
};

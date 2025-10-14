#pragma once

#include "../../src/ecs/Registry.hpp"
#include <cstdint>

/**
 * @brief Handles physics systems and collision detection
 */
class GameInstancePhysics {
public:
    /**
     * @brief Runs all physics systems for one tick
     * @param registry The game registry
     * @param tickDuration Duration of one tick
     * @return Number of platforms that need to be regenerated
     */
    static int updateSystems(Registry& registry, float tickDuration);

    /**
     * @brief Checks all collision systems
     * @param registry The game registry
     * @param tickDuration Duration of one tick
     */
    static void checkCollisions(Registry& registry, float tickDuration);

    /**
     * @brief Updates previous positions for all entities
     * @param registry The game registry
     */
    static void updatePreviousPositions(Registry& registry);
};

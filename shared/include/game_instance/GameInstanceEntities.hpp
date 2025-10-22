#pragma once

#include "../../src/ecs/Entity.hpp"
#include "../../src/ecs/Registry.hpp"
#include "../../src/levels/Level.hpp"
#include <chrono>
#include <cstdint>
#include <vector>

/**
 * @brief Manages entity lifecycle (creation, tracking, cleanup)
 */
class GameInstanceEntities {
public:
    /**
     * @brief Constructs entity manager
     */
    GameInstanceEntities() = default;

    /**
     * @brief Sets the current level configuration
     * @param level The level configuration to use
     */
    void setLevel(const Level* level) { _currentLevel = level; }

    /**
     * @brief Initializes the level with entities
     * @param registry The game registry
     */
    void initializeLevel(Registry& registry);

    /**
     * @brief Cleans up dead and expired entities
     * @param registry The game registry
     * @param tickDuration Duration of one tick
     */
    void cleanupEntities(Registry& registry, float tickDuration);

    /**
     * @brief Gets newly spawned entities and clears the list
     */
    std::vector<Entity> getAndClearNewEntities();

    /**
     * @brief Gets killed entity IDs and clears the list
     */
    std::vector<uint32_t> getAndClearKilledEntities();

    /**
     * @brief Gets the list of entities to add (for modifications during cleanup)
     */
    std::vector<Entity>& getNewEntitiesThisTick() { return _newEntitiesThisTick; }

    /**
     * @brief Gets the list of killed entities (for modifications during cleanup)
     */
    std::vector<uint32_t>& getKilledEntitiesThisTick() { return _killedEntitiesThisTick; }

    /**
     * @brief Gets platforms to add count
     */
    int getPlatformsToAdd() const { return _platformsToAdd; }

    /**
     * @brief Sets platforms to add count
     */
    void setPlatformsToAdd(int count) { _platformsToAdd = count; }

    /**
     * @brief Spawns random power-ups
     * @param registry The game registry
     * @param count Number of power-ups to spawn
     */
    void spawnRandomPowerUps(Registry& registry, int count);

    /**
     * @brief Generates and adds platforms
     * @param registry The game registry
     * @param count Number of platforms to generate
     */
    void generatePlatforms(Registry& registry, int count);

private:
    std::vector<Entity> _newEntitiesThisTick;
    std::vector<uint32_t> _killedEntitiesThisTick;
    int _platformsToAdd = 0;
    std::chrono::steady_clock::time_point _lastPowerUpSpawnTime;
    static constexpr float POWER_UP_SPAWN_INTERVAL = 30.0f;
    const Level* _currentLevel = nullptr;
};

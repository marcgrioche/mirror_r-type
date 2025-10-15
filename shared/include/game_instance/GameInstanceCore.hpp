#pragma once

#include "../Config.hpp"
#include "../../src/ecs/Registry.hpp"
#include <chrono>
#include <cstdint>

/**
 * @brief Core game instance state and lifecycle management
 */
class GameInstanceCore {
public:
    static constexpr uint32_t TICKS_PER_SECOND = 60;
    static constexpr float TICK_DURATION = 1.0f / TICKS_PER_SECOND;

    /**
     * @brief Constructs the core game state
     * @param lobbyId The ID of the lobby this game instance belongs to
     */
    explicit GameInstanceCore(uint32_t lobbyId);

    /**
     * @brief Gets the lobby ID
     */
    uint32_t getLobbyId() const { return _lobbyId; }

    /**
     * @brief Gets the current tick number
     */
    uint32_t getCurrentTick() const { return _currentTick; }

    /**
     * @brief Increments the current tick
     */
    void incrementTick() { _currentTick++; }

    /**
     * @brief Checks if the game is running
     */
    bool isRunning() const { return _isRunning; }

    /**
     * @brief Sets the running state
     */
    void setRunning(bool running) { _isRunning = running; }

    /**
     * @brief Checks if the game has been won
     */
    bool hasWon() const { return _gameWon; }

    /**
     * @brief Sets the won state
     */
    void setWon(bool won) { _gameWon = won; }

    /**
     * @brief Checks if the game has been lost
     */
    bool hasLost() const { return _gameLost; }

    /**
     * @brief Sets the lost state
     */
    void setLost(bool lost) { _gameLost = lost; }

    /**
     * @brief Gets the registry
     */
    Registry& getRegistry() { return _registry; }

    /**
     * @brief Gets the registry (const)
     */
    const Registry& getRegistry() const { return _registry; }

    /**
     * @brief Gets the last tick time
     */
    std::chrono::steady_clock::time_point getLastTickTime() const { return _lastTickTime; }

    /**
     * @brief Sets the last tick time
     */
    void setLastTickTime(std::chrono::steady_clock::time_point time) { _lastTickTime = time; }

    /**
     * @brief Checks if state changed this tick
     */
    bool hasStateChanged();

    /**
     * @brief Marks state as changed
     */
    void markStateChanged() { _stateChanged = true; }

private:
    uint32_t _lobbyId;
    Registry _registry;
    bool _isRunning;
    uint32_t _currentTick;
    std::chrono::steady_clock::time_point _lastTickTime;
    bool _stateChanged;
    bool _gameWon;
    bool _gameLost;
};

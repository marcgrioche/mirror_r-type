/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Input history buffer for client prediction
*/

#pragma once

#include "../../../shared/include/GameInputs.hpp"
#include <cstdint>
#include <deque>
#include <vector>

namespace Client {

/**
 * Represents a single input frame for prediction replay.
 */
struct InputHistoryEntry {
    uint32_t tick;
    std::vector<std::pair<GameInput, bool>> inputs;

    InputHistoryEntry(uint32_t t, const std::vector<std::pair<GameInput, bool>>& i)
        : tick(t)
        , inputs(i)
    {
    }
};

/**
 * Manages input history for client-side prediction and reconciliation.
 */
class InputHistory {
public:
    static constexpr size_t MAX_HISTORY_SIZE = 120; // 2 seconds at 60 TPS
    static constexpr float RECONCILIATION_THRESHOLD = 5.0f; // pixels

    InputHistory() = default;
    ~InputHistory() = default;

    /**
     * Add a new input entry to the history.
     *
     * Args:
     *     tick (uint32_t): Game tick number
     *     inputs (const std::vector<std::pair<GameInput, bool>>&): Input states
     */
    void addInputEntry(uint32_t tick, const std::vector<std::pair<GameInput, bool>>& inputs);

    /**
     * Get all inputs from a specific tick onwards.
     *
     * Args:
     *     fromTick (uint32_t): Starting tick (inclusive)
     *
     * Returns:
     *     std::vector<InputHistoryEntry>: Inputs from specified tick onwards
     */
    std::vector<InputHistoryEntry> getInputsSince(uint32_t fromTick) const;

    /**
     * Remove inputs older than specified tick.
     *
     * Args:
     *     oldestTick (uint32_t): Oldest tick to keep
     */
    void removeInputsOlderThan(uint32_t oldestTick);

    /**
     * Clear all input history.
     */
    void clear();

    /**
     * Get the oldest tick in history.
     *
     * Returns:
     *     uint32_t: Oldest tick, or 0 if empty
     */
    uint32_t getOldestTick() const;

    /**
     * Get the newest tick in history.
     *
     * Returns:
     *     uint32_t: Newest tick, or 0 if empty
     */
    uint32_t getNewestTick() const;

    /**
     * Check if history is empty.
     *
     * Returns:
     *     bool: True if history is empty
     */
    bool isEmpty() const { return m_history.empty(); }

    /**
     * Get current history size.
     *
     * Returns:
     *     size_t: Number of entries in history
     */
    size_t size() const { return m_history.size(); }

private:
    std::deque<InputHistoryEntry> m_history;
};

} // namespace Client

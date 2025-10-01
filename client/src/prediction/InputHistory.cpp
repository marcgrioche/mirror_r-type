/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Input history buffer implementation
*/

#include "../../include/prediction/InputHistory.hpp"
#include <algorithm>

namespace Client {

void InputHistory::addInputEntry(uint32_t tick, const std::vector<std::pair<GameInput, bool>>& inputs)
{
    m_history.emplace_back(tick, inputs);

    while (m_history.size() > MAX_HISTORY_SIZE) {
        m_history.pop_front();
    }
}

std::vector<InputHistoryEntry> InputHistory::getInputsSince(uint32_t fromTick) const
{
    std::vector<InputHistoryEntry> result;

    for (const auto& entry : m_history) {
        if (entry.tick >= fromTick) {
            result.push_back(entry);
        }
    }

    return result;
}

void InputHistory::removeInputsOlderThan(uint32_t oldestTick)
{
    auto it = std::remove_if(m_history.begin(), m_history.end(),
        [oldestTick](const InputHistoryEntry& entry) {
            return entry.tick < oldestTick;
        });

    m_history.erase(it, m_history.end());
}

void InputHistory::clear()
{
    m_history.clear();
}

uint32_t InputHistory::getOldestTick() const
{
    if (m_history.empty()) {
        return 0;
    }
    return m_history.front().tick;
}

uint32_t InputHistory::getNewestTick() const
{
    if (m_history.empty()) {
        return 0;
    }
    return m_history.back().tick;
}

} // namespace Client

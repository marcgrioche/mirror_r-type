/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Input history buffer implementation (DISABLED)
*/

#include "InputHistory.hpp"

void InputHistory::recordInput(const uint32_t t_tick, const std::vector<std::pair<GameInput, bool>>& t_actions)
{
    m_history.push_back({ t_tick, t_actions });
}

const std::vector<InputFrame>& InputHistory::getHistory() const
{
    return m_history;
}

void InputHistory::removeUpToTick(const uint32_t t_tick)
{
    // Remove all frames with tick <= given tick
    while (!m_history.empty() && m_history.front().tick <= t_tick) {
        m_history.erase(m_history.begin());
    }
}

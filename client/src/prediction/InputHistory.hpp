#pragma once
#include <cstdint>
#include <utility>
#include <vector>

#include "GameInputs.hpp"
#include "InputManager.hpp"

// Stores all actions for a tick
struct InputFrame {
    uint32_t tick;
    std::vector<std::pair<GameInput, bool>> actions;
};

class InputHistory {
public:
    void recordInput(uint32_t t_tick, const std::vector<std::pair<GameInput, bool>>& t_actions);
    const std::vector<InputFrame>& getHistory() const;
    void removeUpToTick(uint32_t t_tick); // Remove all inputs up to and including tick
private:
    std::vector<InputFrame> m_history;
};
#pragma once

#include <GameInstance.hpp>
#include <vector>

class PlayerActions {
public:
    static bool updateVelocity(const std::vector<std::pair<GameInput, bool>>& t_inputs,
        std::shared_ptr<Registry> t_registry, Entity& t_player);

private:
    PlayerActions() = delete;
    ~PlayerActions() = delete;
};
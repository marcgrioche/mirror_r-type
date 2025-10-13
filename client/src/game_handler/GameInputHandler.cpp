/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game Input Handler - Manages input gathering and processing
*/

#include "Game.hpp"
#include "managers/ConfigManager.hpp"

std::vector<std::pair<GameInput, bool>> Game::getCurrentInputs()
{
    std::vector<std::pair<GameInput, bool>> inputs;

    inputs.emplace_back(GameInput::UP, _inputs.isActionPressed(GameAction::MOVE_UP));
    inputs.emplace_back(GameInput::DOWN, _inputs.isActionPressed(GameAction::MOVE_DOWN));
    inputs.emplace_back(GameInput::LEFT, _inputs.isActionPressed(GameAction::MOVE_LEFT));
    inputs.emplace_back(GameInput::RIGHT, _inputs.isActionPressed(GameAction::MOVE_RIGHT));

    bool attack = _inputs.isActionPressed(GameAction::SHOOT) || ConfigManager::getInstance().getAutoShoot();
    inputs.emplace_back(GameInput::ATTACK, attack);

    inputs.emplace_back(GameInput::DASH, _inputs.isActionPressed(GameAction::DASH));

    return inputs;
}

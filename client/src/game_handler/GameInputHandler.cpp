/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game Input Handler - Manages input gathering and processing
*/

#include "Game.hpp"

std::vector<std::pair<GameInput, bool>> Game::getCurrentInputs()
{
    std::vector<std::pair<GameInput, bool>> inputs;

    inputs.emplace_back(GameInput::UP, _inputs.isActionPressed(GameAction::MOVE_UP));
    inputs.emplace_back(GameInput::DOWN, _inputs.isActionPressed(GameAction::MOVE_DOWN));
    inputs.emplace_back(GameInput::LEFT, _inputs.isActionPressed(GameAction::MOVE_LEFT));
    inputs.emplace_back(GameInput::RIGHT, _inputs.isActionPressed(GameAction::MOVE_RIGHT));
    inputs.emplace_back(GameInput::ATTACK, _inputs.isActionPressed(GameAction::SHOOT));
    inputs.emplace_back(GameInput::DASH, false);

    return inputs;
}

/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** InputManager
*/

#include "InputManager.hpp"

InputManager& InputManager::getInstance()
{
    static InputManager instance;
    instance.setDefaultKeyBindings();
    return instance;
}

void InputManager::updateInputs(SDL_Event& e)
{
    previousActionStates = actionStates;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            updateActionState(GameAction::QUIT, true);
        } else if (e.type == SDL_KEYDOWN) {
            auto it = keyToAction.find(e.key.keysym.sym);
            if (it != keyToAction.end()) {
                updateActionState(it->second, true);
            }
        } else if (e.type == SDL_KEYUP) {
            auto it = keyToAction.find(e.key.keysym.sym);
            if (it != keyToAction.end()) {
                updateActionState(it->second, false);
            }
        }
    }
}

void InputManager::bindKey(SDL_Keycode key, GameAction action)
{
    keyToAction[key] = action;
}

void InputManager::unbindKey(SDL_Keycode key)
{
    keyToAction.erase(key);
}

bool InputManager::isActionPressed(GameAction action) const
{
    std::unordered_map<GameAction, bool>::const_iterator it = actionStates.find(action);
    return it != actionStates.end() && it->second;
}

bool InputManager::isActionJustPressed(GameAction action) const
{
    std::unordered_map<GameAction, bool>::const_iterator currentIt = actionStates.find(action);
    std::unordered_map<GameAction, bool>::const_iterator previousIt = previousActionStates.find(action);

    bool currentState = (currentIt != actionStates.end()) ? currentIt->second : false;
    bool previousState = (previousIt != previousActionStates.end()) ? previousIt->second : false;

    return currentState && !previousState;
}

bool InputManager::isActionJustReleased(GameAction action) const
{
    std::unordered_map<GameAction, bool>::const_iterator currentIt = actionStates.find(action);
    std::unordered_map<GameAction, bool>::const_iterator previousIt = previousActionStates.find(action);

    bool currentState = (currentIt != actionStates.end()) ? currentIt->second : false;
    bool previousState = (previousIt != previousActionStates.end()) ? previousIt->second : false;

    return !currentState && previousState;
}

void InputManager::setDefaultKeyBindings()
{
    keyToAction.clear();

    bindKey(SDLK_z, GameAction::MOVE_UP);
    bindKey(SDLK_s, GameAction::MOVE_DOWN);
    bindKey(SDLK_q, GameAction::MOVE_LEFT);
    bindKey(SDLK_d, GameAction::MOVE_RIGHT);
    bindKey(SDLK_SPACE, GameAction::SHOOT);
    bindKey(SDLK_ESCAPE, GameAction::QUIT);

    bindKey(SDLK_UP, GameAction::MOVE_UP);
    bindKey(SDLK_DOWN, GameAction::MOVE_DOWN);
    bindKey(SDLK_LEFT, GameAction::MOVE_LEFT);
    bindKey(SDLK_RIGHT, GameAction::MOVE_RIGHT);
}

void InputManager::updateActionState(GameAction action, bool isPressed)
{
    actionStates[action] = isPressed;
}

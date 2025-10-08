/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** InputManager
*/

#include "InputManager.hpp"
#include "ConfigManager.hpp"

InputManager& InputManager::getInstance()
{
    static InputManager instance;
    return instance;
}

void InputManager::updateInputs(SDL_Event& e)
{
    beginFrame();
    while (SDL_PollEvent(&e) != 0) {
        handleSDLEvent(e);
    }
}

void InputManager::beginFrame()
{
    previousActionStates = actionStates;
}

void InputManager::handleSDLEvent(const SDL_Event& e)
{
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

void InputManager::bindKey(SDL_Keycode key, GameAction action)
{
    keyToAction[key] = action;
}

void InputManager::bindKeysForAction(GameAction action, const std::vector<SDL_Keycode>& keys)
{
    for (SDL_Keycode key : keys) {
        bindKey(key, action);
    }
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

    ConfigManager& config = ConfigManager::getInstance();

    bindKeysForAction(GameAction::MOVE_UP, config.getKeysForAction(GameAction::MOVE_UP));
    bindKeysForAction(GameAction::MOVE_DOWN, config.getKeysForAction(GameAction::MOVE_DOWN));
    bindKeysForAction(GameAction::MOVE_LEFT, config.getKeysForAction(GameAction::MOVE_LEFT));
    bindKeysForAction(GameAction::MOVE_RIGHT, config.getKeysForAction(GameAction::MOVE_RIGHT));
    bindKeysForAction(GameAction::SHOOT, config.getKeysForAction(GameAction::SHOOT));
    bindKeysForAction(GameAction::QUIT, config.getKeysForAction(GameAction::QUIT));
    bindKeysForAction(GameAction::DASH, config.getKeysForAction(GameAction::DASH));

    bindKey(SDLK_UP, GameAction::MOVE_UP);
    bindKey(SDLK_DOWN, GameAction::MOVE_DOWN);
    bindKey(SDLK_LEFT, GameAction::MOVE_LEFT);
    bindKey(SDLK_RIGHT, GameAction::MOVE_RIGHT);
}

void InputManager::updateActionState(GameAction action, bool isPressed)
{
    actionStates[action] = isPressed;
}

void InputManager::reloadKeyBindings()
{
    setDefaultKeyBindings();
}

bool InputManager::saveKeyBindings()
{
    ConfigManager& config = ConfigManager::getInstance();

    std::unordered_map<GameAction, std::vector<SDL_Keycode>> actionToKeys;
    for (const auto& binding : keyToAction) {
        actionToKeys[binding.second].push_back(binding.first);
    }

    for (const auto& actionKeys : actionToKeys) {
        config.setKeysForAction(actionKeys.first, actionKeys.second);
    }

    return config.saveKeyBindings();
}

const std::unordered_map<GameAction, bool>& InputManager::getActions() const
{
    return actionStates;
}

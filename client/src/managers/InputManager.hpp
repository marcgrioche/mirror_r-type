/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** InputManager
*/

#pragma once

#include <SDL.h>
#include <unordered_map>

enum class GameAction {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    SHOOT,
    QUIT,
    ACTION_COUNT
};

class InputManager {
public:
    static InputManager& getInstance();

    void updateInputs(SDL_Event& e);

    void bindKey(SDL_Keycode key, GameAction action);
    void unbindKey(SDL_Keycode key);
    bool isActionPressed(GameAction action) const;
    bool isActionJustPressed(GameAction action) const;
    bool isActionJustReleased(GameAction action) const;
    void setDefaultKeyBindings();

    bool isUp() const { return isActionPressed(GameAction::MOVE_UP); }
    bool isDown() const { return isActionPressed(GameAction::MOVE_DOWN); }
    bool isLeft() const { return isActionPressed(GameAction::MOVE_LEFT); }
    bool isRight() const { return isActionPressed(GameAction::MOVE_RIGHT); }
    bool isShoot() const { return isActionPressed(GameAction::SHOOT); }
    bool isQuit() const { return isActionPressed(GameAction::QUIT); }

private:
    InputManager() { setDefaultKeyBindings(); }
    ~InputManager() = default;

    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    std::unordered_map<SDL_Keycode, GameAction> keyToAction;
    std::unordered_map<GameAction, bool> actionStates;
    std::unordered_map<GameAction, bool> previousActionStates;

    void updateActionState(GameAction action, bool isPressed);
};

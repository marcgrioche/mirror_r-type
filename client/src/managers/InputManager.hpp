/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** InputManager
*/

#pragma once

#include <SDL.h>
#include <unordered_map>
#include <vector>

enum class GameAction {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    SHOOT,
    DASH,
    QUIT,
    ACTION_COUNT
};

constexpr SDL_Keycode MOUSE_LEFT_KEYCODE = static_cast<SDL_Keycode>(0x40000000);
constexpr SDL_Keycode MOUSE_RIGHT_KEYCODE = static_cast<SDL_Keycode>(0x40000001);

class InputManager {
public:
    /**
     * @brief Gets the singleton instance of InputManager
     * @return Reference to the singleton InputManager instance
     */
    static InputManager& getInstance();

    /**
     * @brief Updates input state from an SDL event (deprecated - use handleSDLEvent + beginFrame instead)
     * @param e The SDL event to process
     */
    void updateInputs(SDL_Event& e);

    /**
     * @brief Marks the start of a new frame, capturing previous action states for edge detection
     */
    void beginFrame();

    /**
     * @brief Updates input state from a single SDL event (key up/down, quit)
     * @param e The SDL event to process
     */
    void handleSDLEvent(const SDL_Event& e);

    /**
     * @brief Binds a keyboard key to a game action
     * @param key The SDL keycode to bind
     * @param action The game action to bind to
     */
    void bindKey(SDL_Keycode key, GameAction action);

    /**
     * @brief Binds multiple keys to a single game action
     * @param action The game action to bind keys to
     * @param keys Vector of SDL keycodes to bind
     */
    void bindKeysForAction(GameAction action, const std::vector<SDL_Keycode>& keys);

    /**
     * @brief Unbinds a keyboard key from any game action
     * @param key The SDL keycode to unbind
     */
    void unbindKey(SDL_Keycode key);

    /**
     * @brief Checks if a game action is currently pressed
     * @param action The game action to check
     * @return True if the action is pressed, false otherwise
     */
    bool isActionPressed(GameAction action) const;

    /**
     * @brief Checks if a game action was just pressed this frame
     * @param action The game action to check
     * @return True if the action was just pressed, false otherwise
     */
    bool isActionJustPressed(GameAction action) const;

    /**
     * @brief Checks if a game action was just released this frame
     * @param action The game action to check
     * @return True if the action was just released, false otherwise
     */
    bool isActionJustReleased(GameAction action) const;

    /**
     * @brief Sets default key bindings for all game actions
     */
    void setDefaultKeyBindings();

    /**
     * @brief Reloads key bindings from configuration file
     */
    void reloadKeyBindings();

    /**
     * @brief Saves current key bindings to configuration file
     * @return True if save succeeded, false otherwise
     */
    bool saveKeyBindings();

    bool isUp() const { return isActionPressed(GameAction::MOVE_UP); }
    bool isDown() const { return isActionPressed(GameAction::MOVE_DOWN); }
    bool isLeft() const { return isActionPressed(GameAction::MOVE_LEFT); }
    bool isRight() const { return isActionPressed(GameAction::MOVE_RIGHT); }
    bool isShoot() const { return isActionPressed(GameAction::SHOOT); }
    bool isDash() const { return isActionPressed(GameAction::DASH); }
    bool isQuit() const { return isActionPressed(GameAction::QUIT); }
    const std::unordered_map<GameAction, bool>& getActions() const;

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

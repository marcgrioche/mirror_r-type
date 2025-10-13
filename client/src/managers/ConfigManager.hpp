/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** ConfigManager
*/

#pragma once

#include <SDL.h>
#include <string>
#include <unordered_map>
#include <vector>

enum class GameAction;

class ConfigManager {
public:
    /**
     * @brief Gets the singleton instance of ConfigManager
     * @return Reference to the singleton ConfigManager instance
     */
    static ConfigManager& getInstance();

    /**
     * @brief Loads key bindings from the configuration file
     * @return True if key bindings were loaded successfully, false otherwise
     */
    bool loadKeyBindings();

    /**
     * @brief Saves current key bindings to the configuration file
     * @return True if key bindings were saved successfully, false otherwise
     */
    bool saveKeyBindings();

    /**
     * @brief Gets the key codes bound to a specific game action
     * @param action The game action to get keys for
     * @return Vector of SDL key codes bound to the action
     */
    const std::vector<SDL_Keycode>& getKeysForAction(GameAction action) const;

    /**
     * @brief Sets the key codes for a specific game action (replaces existing keys)
     * @param action The game action to set keys for
     * @param keys Vector of SDL key codes to bind to the action
     */
    void setKeysForAction(GameAction action, const std::vector<SDL_Keycode>& keys);

    /**
     * @brief Adds a key code to the existing keys for a game action
     * @param action The game action to add a key to
     * @param key The SDL key code to add
     */
    void addKeyForAction(GameAction action, SDL_Keycode key);

    /**
     * @brief Removes a key code from the keys bound to a game action
     * @param action The game action to remove a key from
     * @param key The SDL key code to remove
     */
    void removeKeyForAction(GameAction action, SDL_Keycode key);

    /**
     * @brief Resets all key bindings to their default values
     */
    void resetToDefaults();

private:
    ConfigManager();
    ~ConfigManager() = default;

    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    std::string getConfigFilePath() const;
    void setDefaultKeyBindings();

    std::unordered_map<GameAction, std::vector<SDL_Keycode>> m_keyBindings;
    const std::string m_configFileName = "keybindings.ini";
};

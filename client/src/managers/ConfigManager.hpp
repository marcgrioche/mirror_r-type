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
    static ConfigManager& getInstance();

    bool loadKeyBindings();

    bool saveKeyBindings();

    const std::vector<SDL_Keycode>& getKeysForAction(GameAction action) const;

    void setKeysForAction(GameAction action, const std::vector<SDL_Keycode>& keys);

    void addKeyForAction(GameAction action, SDL_Keycode key);

    void removeKeyForAction(GameAction action, SDL_Keycode key);

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

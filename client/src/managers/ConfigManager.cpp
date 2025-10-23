/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** ConfigManager
*/

#include "ConfigManager.hpp"
#include "InputManager.hpp"
#include <SDL.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

ConfigManager& ConfigManager::getInstance()
{
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager()
{
    setDefaultKeyBindings();
    loadKeyBindings();
}

bool ConfigManager::loadKeyBindings()
{
    std::string configPath = getConfigFilePath();

    if (!std::filesystem::exists(configPath)) {
        std::cout << "Config file not found, using defaults: " << configPath << std::endl;
        return false;
    }

    std::ifstream file(configPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << configPath << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        std::string actionStr, keyStr;

        if (std::getline(iss, actionStr, '=') && std::getline(iss, keyStr)) {
            actionStr.erase(actionStr.find_last_not_of(" \t") + 1);
            actionStr.erase(0, actionStr.find_first_not_of(" \t"));
            keyStr.erase(keyStr.find_last_not_of(" \t") + 1);
            keyStr.erase(0, keyStr.find_first_not_of(" \t"));

            GameAction action = GameAction::ACTION_COUNT;
            if (actionStr == "move_up")
                action = GameAction::MOVE_UP;
            else if (actionStr == "move_down")
                action = GameAction::MOVE_DOWN;
            else if (actionStr == "move_left")
                action = GameAction::MOVE_LEFT;
            else if (actionStr == "move_right")
                action = GameAction::MOVE_RIGHT;
            else if (actionStr == "shoot")
                action = GameAction::SHOOT;
            else if (actionStr == "quit")
                action = GameAction::QUIT;

            if (actionStr == "auto_shoot") {
                try {
                    int value = std::stoi(keyStr);
                    m_autoShoot = (value != 0);
                } catch (const std::exception&) {
                    std::cerr << "Invalid value for auto_shoot: " << keyStr << std::endl;
                }
            } else if (actionStr == "colorblind_type") {
                if (keyStr == "protanopia" || keyStr == "deuteranopia" || keyStr == "tritanopia" || keyStr.empty()) {
                    m_colorblindType = keyStr;
                } else {
                    std::cerr << "Invalid value for colorblind_type: " << keyStr << std::endl;
                }
            } else if (action != GameAction::ACTION_COUNT) {
                std::vector<SDL_Keycode> keys;
                std::istringstream keyStream(keyStr);
                std::string keyToken;

                while (std::getline(keyStream, keyToken, ',')) {
                    keyToken.erase(keyToken.find_last_not_of(" \t") + 1);
                    keyToken.erase(0, keyToken.find_first_not_of(" \t"));

                    try {
                        int keycode = std::stoi(keyToken);
                        keys.push_back(static_cast<SDL_Keycode>(keycode));
                    } catch (const std::exception&) {
                        std::cerr << "Invalid keycode for action " << actionStr << ": " << keyToken << std::endl;
                    }
                }

                if (!keys.empty()) {
                    m_keyBindings[action] = keys;
                }
            }
        }
    }

    file.close();
    std::cout << "Loaded keybindings from: " << configPath << std::endl;
    return true;
}

bool ConfigManager::saveKeyBindings()
{
    std::string configPath = getConfigFilePath();

    std::filesystem::path dir = std::filesystem::path(configPath).parent_path();
    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directories(dir);
    }

    std::ofstream file(configPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file for writing: " << configPath << std::endl;
        return false;
    }

    file << "# R-Type Keybindings Configuration" << std::endl;
    file << "# Format: action=keycode1,keycode2,..." << std::endl;
    file << std::endl;

    for (const auto& binding : m_keyBindings) {
        std::string actionStr;
        switch (binding.first) {
        case GameAction::MOVE_UP:
            actionStr = "move_up";
            break;
        case GameAction::MOVE_DOWN:
            actionStr = "move_down";
            break;
        case GameAction::MOVE_LEFT:
            actionStr = "move_left";
            break;
        case GameAction::MOVE_RIGHT:
            actionStr = "move_right";
            break;
        case GameAction::SHOOT:
            actionStr = "shoot";
            break;
        case GameAction::QUIT:
            actionStr = "quit";
            break;
        default:
            continue;
        }

        file << actionStr << "=";
        for (size_t i = 0; i < binding.second.size(); ++i) {
            if (i > 0)
                file << ",";
            file << static_cast<int>(binding.second[i]);
        }
        file << std::endl;
    }

    file << "auto_shoot=" << (m_autoShoot ? "1" : "0") << std::endl;
    file << "colorblind_type=" << m_colorblindType << std::endl;

    file.close();
    std::cout << "Saved keybindings to: " << configPath << std::endl;
    return true;
}

const std::vector<SDL_Keycode>& ConfigManager::getKeysForAction(GameAction action) const
{
    static const std::vector<SDL_Keycode> emptyVector;
    auto it = m_keyBindings.find(action);
    return (it != m_keyBindings.end()) ? it->second : emptyVector;
}

void ConfigManager::setKeysForAction(GameAction action, const std::vector<SDL_Keycode>& keys)
{
    m_keyBindings[action] = keys;
}

void ConfigManager::addKeyForAction(GameAction action, SDL_Keycode key)
{
    m_keyBindings[action].push_back(key);
}

void ConfigManager::removeKeyForAction(GameAction action, SDL_Keycode key)
{
    auto& keys = m_keyBindings[action];
    keys.erase(std::remove(keys.begin(), keys.end(), key), keys.end());
}

void ConfigManager::resetToDefaults()
{
    setDefaultKeyBindings();
}

std::string ConfigManager::getConfigFilePath() const
{
    return "client/res/config/" + m_configFileName;
}

void ConfigManager::setDefaultKeyBindings()
{
    m_keyBindings.clear();

    m_keyBindings[GameAction::MOVE_UP] = { SDLK_z };
    m_keyBindings[GameAction::MOVE_DOWN] = { SDLK_s };
    m_keyBindings[GameAction::MOVE_LEFT] = { SDLK_q };
    m_keyBindings[GameAction::MOVE_RIGHT] = { SDLK_d };
    m_keyBindings[GameAction::SHOOT] = { SDLK_SPACE };
    m_keyBindings[GameAction::QUIT] = { SDLK_ESCAPE };
    m_keyBindings[GameAction::DASH] = { SDLK_LSHIFT };
}

bool ConfigManager::getAutoShoot() const
{
    return m_autoShoot;
}

void ConfigManager::setAutoShoot(bool enabled)
{
    m_autoShoot = enabled;
}

std::string ConfigManager::getColorblindType() const
{
    return m_colorblindType;
}

void ConfigManager::setColorblindType(const std::string& type)
{
    if (type == "protanopia" || type == "deuteranopia" || type == "tritanopia" || type.empty()) {
        m_colorblindType = type;
    }
}

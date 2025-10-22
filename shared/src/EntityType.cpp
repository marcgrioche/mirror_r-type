/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Entity Type Implementation
*/

#include "../include/EntityType.hpp"
#include <stdexcept>
#include <unordered_map>

EntityType stringToEntityType(const std::string& str)
{
    static const std::unordered_map<std::string, EntityType> stringToTypeMap = {
        { "PLAYER", EntityType::PLAYER },
        { "PROJECTILE", EntityType::PROJECTILE },
        { "PLATFORM", EntityType::PLATFORM },
        { "ENEMY", EntityType::ENEMY },
        { "BOSS", EntityType::BOSS },
        { "POWERUP", EntityType::POWERUP }
    };

    auto it = stringToTypeMap.find(str);
    if (it == stringToTypeMap.end()) {
        throw std::invalid_argument("Unknown entity type: " + str);
    }
    return it->second;
}

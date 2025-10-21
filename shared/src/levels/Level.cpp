/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Level
*/

#include "Level.hpp"
#include <fstream>
#include <iostream>

Level::Level(/* args */)
{
}

Level::~Level()
{
}

void Level::loadFromJson(const std::string& filepath)
{
    try {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open level file: " << filepath << std::endl;
            return;
        }

        nlohmann::json config;
        file >> config;

        if (config.contains("platform")) {
            auto& platform = config["platform"];
            _platformLayers = platform.value("layers", _platformLayers);
            _platformTopMargin = platform.value("topMargin", _platformTopMargin);
            _platformBottomMargin = platform.value("bottomMargin", _platformBottomMargin);
            _platformSameLevelEpsilon = platform.value("sameLevelEpsilon", _platformSameLevelEpsilon);
            _platformMinDistXSame = platform.value("minDistXSame", _platformMinDistXSame);
            _platformCrossDistX = platform.value("crossDistX", _platformCrossDistX);
            _platformMaxDiffY = platform.value("maxDiffY", _platformMaxDiffY);
            _platformReachableX = platform.value("reachableX", _platformReachableX);
            _platformVelocityX = platform.value("velocityX", _platformVelocityX);
            _platformVelocityY = platform.value("velocityY", _platformVelocityY);
        }
        if (config.contains("enemy")) {
            auto& enemy = config["enemy"];
            _enemyVelocityX = enemy.value("velocityX", _enemyVelocityX);
            _enemyVelocityY = enemy.value("velocityY", _enemyVelocityY);
            _enemyHealth = enemy.value("health", _enemyHealth);
            _enemyWidth = enemy.value("width", _enemyWidth);
            _enemyHeight = enemy.value("height", _enemyHeight);
            _enemyProjectileSpeed = enemy.value("projectileSpeed", _enemyProjectileSpeed);
            _enemyProjectileDamage = enemy.value("projectileDamage", _enemyProjectileDamage);
        }
        if (config.contains("boss")) {
            auto& boss = config["boss"];
            _bossHealth = boss.value("health", _bossHealth);
            _bossWidth = boss.value("width", _bossWidth);
            _bossHeight = boss.value("height", _bossHeight);
            _bossShootFrequency = boss.value("shootFrequency", _bossShootFrequency);
            _bossSpawnEnemyFrequency = boss.value("spawnEnemyFrequency", _bossSpawnEnemyFrequency);
            _bossProjectileSpeed = boss.value("projectileSpeed", _bossProjectileSpeed);
            _bossProjectileDamage = boss.value("projectileDamage", _bossProjectileDamage);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading level configuration: " << e.what() << std::endl;
    }
}

/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Level
*/

#pragma once

#include <nlohmann/json.hpp>
#include <string>

/**
 * @brief Represents a game level with physics properties
 */
class Level {
private:
    float gravity = 1.0f; /**< Gravity value for the level */

    // Platform generation parameters
    int _platformLayers = 8;
    float _platformTopMargin = 80.0f;
    float _platformBottomMargin = 100.0f;
    float _platformSameLevelEpsilon = 28.0f;
    float _platformMinDistXSame = 200.0f;
    float _platformCrossDistX = 180.0f;
    float _platformMaxDiffY = 110.0f;
    float _platformReachableX = 270.0f;
    float _platformVelocityX = -30.0f;
    float _platformVelocityY = 0.0f;

    // Enemy parameters
    float _enemyVelocityX = -80.0f;
    float _enemyVelocityY = 0.0f;
    int _enemyHealth = 20;
    float _enemyWidth = 40.0f;
    float _enemyHeight = 40.0f;
    float _enemyProjectileSpeed = -300.0f;
    int _enemyProjectileDamage = 10;

    // Boss parameters
    int _bossHealth = 200;
    float _bossWidth = 100.0f;
    float _bossHeight = 100.0f;
    float _bossShootFrequency = 2.0f;
    float _bossSpawnEnemyFrequency = 5.0f;
    float _bossProjectileSpeed = -300.0f;
    int _bossProjectileDamage = 10;

public:
    /**
     * @brief Constructs a Level instance
     */
    Level(/* args */);

    /**
     * @brief Destroys the Level instance
     */
    ~Level();

    /**
     * @brief Loads level configuration from a JSON file
     * @param filepath Path to the JSON configuration file
     */
    void loadFromJson(const std::string& filepath);

    // Getters for platform parameters
    int getPlatformLayers() const { return _platformLayers; }
    float getPlatformTopMargin() const { return _platformTopMargin; }
    float getPlatformBottomMargin() const { return _platformBottomMargin; }
    float getPlatformSameLevelEpsilon() const { return _platformSameLevelEpsilon; }
    float getPlatformMinDistXSame() const { return _platformMinDistXSame; }
    float getPlatformCrossDistX() const { return _platformCrossDistX; }
    float getPlatformMaxDiffY() const { return _platformMaxDiffY; }
    float getPlatformReachableX() const { return _platformReachableX; }
    float getPlatformVelocityX() const { return _platformVelocityX; }
    float getPlatformVelocityY() const { return _platformVelocityY; }
    // --- Getters for new parameters ---
    float getEnemyVelocityX() const { return _enemyVelocityX; }
    float getEnemyVelocityY() const { return _enemyVelocityY; }
    int getEnemyHealth() const { return _enemyHealth; }
    float getEnemyWidth() const { return _enemyWidth; }
    float getEnemyHeight() const { return _enemyHeight; }
    float getEnemyProjectileSpeed() const { return _enemyProjectileSpeed; }
    int getEnemyProjectileDamage() const { return _enemyProjectileDamage; }
    int getBossHealth() const { return _bossHealth; }
    float getBossWidth() const { return _bossWidth; }
    float getBossHeight() const { return _bossHeight; }
    float getBossShootFrequency() const { return _bossShootFrequency; }
    float getBossSpawnEnemyFrequency() const { return _bossSpawnEnemyFrequency; }
    float getBossProjectileSpeed() const { return _bossProjectileSpeed; }
    int getBossProjectileDamage() const { return _bossProjectileDamage; }
};

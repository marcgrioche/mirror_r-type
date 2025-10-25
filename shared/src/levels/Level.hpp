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
    float _enemyProjectileWidth = 32.0f;
    float _enemyProjectileHeight = 32.0f;

    // Boss parameters
    int _bossHealth = 200;
    float _bossWidth = 100.0f;
    float _bossHeight = 100.0f;
    float _bossShootFrequency = 2.0f;
    float _bossSpawnEnemyFrequency = 5.0f;
    float _bossProjectileSpeed = -300.0f;
    int _bossProjectileDamage = 10;
    float _bossProjectileWidth = 32.0f;
    float _bossProjectileHeight = 32.0f;
    int _bossHealthStatesNumber = 3;
    std::string _bossIdlePath = "";
    std::vector<std::string> _bossAttackPaths = {};
    std::string _bossProjectilePath = "";
    int _bossFrameWidth = 512;
    int _bossFrameHeight = 512;
    int _bossFramesNb = 8;
    float _bossFrameDuration = 0.17f;
    float _bossSizeFactor = 1.25f;
    float _bossPosXFactor = 0.75f;
    float _bossPosYFactor = 0.0f;

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
    float getEnemyProjectileWidth() const { return _enemyProjectileWidth; }
    float getEnemyProjectileHeight() const { return _enemyProjectileHeight; }
    float getBossProjectileWidth() const { return _bossProjectileWidth; }
    float getBossProjectileHeight() const { return _bossProjectileHeight; }
    int getBossHealthStatesNumber() const { return _bossHealthStatesNumber; }
    const std::string& getBossIdlePath() const { return _bossIdlePath; }
    const std::vector<std::string>& getBossAttackPaths() const { return _bossAttackPaths; }
    const std::string& getBossProjectilePath() const { return _bossProjectilePath; }
    int getBossFrameWidth() const { return _bossFrameWidth; }
    int getBossFrameHeight() const { return _bossFrameHeight; }
    int getBossFramesNb() const { return _bossFramesNb; }
    float getBossFrameDuration() const { return _bossFrameDuration; }
    float getBossSizeFactor() const { return _bossSizeFactor; }
    float getBossPosXFactor() const { return _bossPosXFactor; }
    float getBossPosYFactor() const { return _bossPosYFactor; }
};

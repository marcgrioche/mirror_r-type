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
            _platformSpritePath = platform.value("sprite_path", _platformSpritePath);
            _platformWidth = platform.value("width", _platformWidth);
            _platformHeight = platform.value("height", _platformHeight);
            _platformSizeFactor = platform.value("size_factor", _platformSizeFactor);
        }
        if (config.contains("enemy")) {
            auto& enemy = config["enemy"];
            _enemyVelocityX = enemy.value("velocityX", _enemyVelocityX);
            _enemyVelocityY = enemy.value("velocityY", _enemyVelocityY);
            _enemyHealth = enemy.value("health", _enemyHealth);
            _enemyWidth = enemy.value("width", _enemyWidth);

            // Enemy animation parameters
            _enemyFrameWidth = enemy.value("frame_width", _enemyFrameWidth);
            _enemyFrameHeight = enemy.value("frame_height", _enemyFrameHeight);
            _enemyFramesNb = enemy.value("frames_nb", _enemyFramesNb);
            _enemyFrameDuration = enemy.value("frame_duration", _enemyFrameDuration);
            _enemySizeFactor = enemy.value("size_factor", _enemySizeFactor);
            _enemyHeight = enemy.value("height", _enemyHeight);

        if (config.contains("projectile")) {
            auto& proj = config["projectile"];
            _enemyProjectileSpriteId = proj.value("sprite_path", _enemyProjectileSpriteId);
            _projectileFrameWidth = proj.value("frame_width", _projectileFrameWidth);
            _projectileFrameHeight = proj.value("frame_height", _projectileFrameHeight);
            _projectileFramesNb = proj.value("frames_nb", _projectileFramesNb);
            _projectileFrameDuration = proj.value("frame_duration", _projectileFrameDuration);
            _projectileSizeFactor = proj.value("size_factor", _projectileSizeFactor);
        }
            _enemyProjectileSpeed = enemy.value("projectileSpeed", _enemyProjectileSpeed);
            _enemyProjectileDamage = enemy.value("projectileDamage", _enemyProjectileDamage);
            _enemyProjectileWidth = enemy.value("projectileWidth", _enemyProjectileWidth);
            _enemyProjectileHeight = enemy.value("projectileHeight", _enemyProjectileHeight);
            _enemySpriteId = enemy.value("sprite_path", _enemySpriteId);
            // _enemyProjectileSpriteId = enemy.value("projectile_sprite_path", _enemyProjectileSpriteId);
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
            _bossProjectileWidth = boss.value("projectileWidth", _bossProjectileWidth);
            _bossProjectileHeight = boss.value("projectileHeight", _bossProjectileHeight);
            _bossHealthStatesNumber = boss.value("health_states_number", _bossHealthStatesNumber);
            _bossIdlePath = boss.value("idle_path", _bossIdlePath);
            if (boss.contains("attack_path") && boss["attack_path"].is_array()) {
                _bossAttackPaths.clear();
                for (const auto& path : boss["attack_path"]) {
                    _bossAttackPaths.push_back(path);
                }
            }
            _bossProjectilePath = boss.value("projectile_path", _bossProjectilePath);
            _bossFrameWidth = boss.value("frame_width", _bossFrameWidth);
            _bossFrameHeight = boss.value("frame_height", _bossFrameHeight);
            _bossFramesNb = boss.value("frames_nb", _bossFramesNb);
            _bossFrameDuration = boss.value("frame_duration", _bossFrameDuration);
            _bossSizeFactor = boss.value("size_factor", _bossSizeFactor);
            _bossPosXFactor = boss.value("posX_factor", _bossPosXFactor);
            _bossPosYFactor = boss.value("posY_factor", _bossPosYFactor);
        }

        // Optional parallax background texture IDs
        if (config.contains("parallax_background")) {
            auto& bg = config["parallax_background"];
            _parallaxDownLayerId = bg.value("down_layer", _parallaxDownLayerId);
            _parallaxMiddleLayerId = bg.value("middle_layer", _parallaxMiddleLayerId);
            _parallaxTopLayerId = bg.value("top_layer", _parallaxTopLayerId);
            _parallaxSkyId = bg.value("sky", _parallaxSkyId);
            _parallaxLightId = bg.value("light", _parallaxLightId);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading level configuration: " << e.what() << std::endl;
    }
}

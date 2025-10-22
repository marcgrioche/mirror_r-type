/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SpriteRegistry - Hardcoded sprite configuration registry
*/

#pragma once
#include <string>
#include <unordered_map>

struct SpriteConfig {
    std::string textureId;
    std::string filePath;
    bool animated = false;
    float width = 0.0f;
    float height = 0.0f;
    int frames = 1;
    float frameWidth = 0.0f;
    float frameHeight = 0.0f;
    float frameDuration = 0.1f;
};

class SpriteRegistry {
public:
    static const SpriteConfig* get(const std::string& key);
    static const std::unordered_map<std::string, SpriteConfig>& getAll();

private:
    static const std::unordered_map<std::string, SpriteConfig> sprites;
};

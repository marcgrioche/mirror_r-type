/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SpriteRegistry - Hardcoded sprite configuration registry
*/

#pragma once
#include <string>
#include <unordered_map>

/**
 * @brief Configuration data for a single sprite asset
 */
struct SpriteConfig {
    std::string textureId;     // Texture ID used in ResourceManager
    std::string filePath;      // File path to the texture
    bool animated = false;     // Whether the sprite is animated
    float width = 0.0f;        // Width for static sprites
    float height = 0.0f;       // Height for static sprites
    int frames = 1;            // Number of animation frames
    float frameWidth = 0.0f;   // Width of each frame (for animated sprites)
    float frameHeight = 0.0f;  // Height of each frame (for animated sprites)
    float frameDuration = 0.1f; // Duration of each frame in seconds
};

/**
 * @brief Hardcoded registry of all sprite configurations
 * Simple, typesafe, and integrated directly in the code
 */
class SpriteRegistry {
public:
    /**
     * @brief Gets the sprite configuration by key
     * @param key The key name of the sprite (e.g., "player", "enemy_bydo")
     * @return Pointer to the sprite config, or nullptr if not found
     */
    static const SpriteConfig* get(const std::string& key);

    /**
     * @brief Gets all sprite configurations
     * @return Map of all sprite configurations
     */
    static const std::unordered_map<std::string, SpriteConfig>& getAll();

private:
    static const std::unordered_map<std::string, SpriteConfig> sprites;
};

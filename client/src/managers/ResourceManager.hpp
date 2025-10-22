#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <unordered_map>

class ResourceManager {
public:
    /**
     * @brief Gets the singleton instance of ResourceManager
     * @return Reference to the singleton ResourceManager instance
     */
    static ResourceManager& getInstance();

    /**
     * @brief Loads a texture from file and stores it with the given ID
     * @param renderer The SDL renderer to create the texture with
     * @param id Unique identifier for the texture
     * @param path File path to the image file
     * @return Pointer to the loaded SDL texture, or nullptr on failure
     */
    SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& id, const std::string& path);

    /**
     * @brief Retrieves a previously loaded texture by ID, loading it if not already cached
     * @param renderer The SDL renderer to create the texture with (if loading)
     * @param id The unique identifier of the texture
     * @param path File path to the image file (if loading)
     * @return Pointer to the SDL texture, or nullptr if loading fails
     */
    SDL_Texture* getOrLoadTexture(SDL_Renderer* renderer, const std::string& id, const std::string& path);

    /**
     * @brief Retrieves a previously loaded texture by ID
     * @param id The unique identifier of the texture
     * @return Pointer to the SDL texture, or nullptr if not found
     */
    SDL_Texture* getTexture(const std::string& id) const;

    /**
     * @brief Loads all textures from the sprite registry
     * @param renderer The SDL renderer to create textures with
     * @return True if all textures loaded successfully, false otherwise
     */
    bool loadFromRegistry(SDL_Renderer* renderer);

    /**
     * @brief Loads a font at the specified size
     * @param t_fontPath Path to the font file
     * @param t_fontSize Size of the font to load
     */
    void loadFonts(const std::string& t_fontPath, int t_fontSize);

    /**
     * @brief Retrieves a previously loaded font by size
     * @param t_fontSize The size of the font to retrieve
     * @return Pointer to the TTF font, or nullptr if not found
     */
    TTF_Font* getFont(int t_fontSize);

    /**
     * @brief Unloads all loaded textures and fonts, freeing memory
     */
    void unloadAll();

private:
    ResourceManager() = default;
    ~ResourceManager();

    std::unordered_map<std::string, SDL_Texture*> textures;
    std::string defaultFontPath = "client/res/fonts/OpenSans-Medium.ttf";
    std::unordered_map<int, TTF_Font*> fonts;
};

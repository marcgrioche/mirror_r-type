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
     * @brief Initializes the resource manager with the base path
     * Should be called after SDL initialization
     */
    void initialize();

    /**
     * @brief Gets the full path for an asset relative to the executable
     * @param relativePath Path relative to the res/ directory
     * @return Full path to the asset
     */
    std::string getAssetPath(const std::string& relativePath) const;

    /**
     * @brief Gets the base path where the executable is located
     * @return Base path to the executable directory
     */
    std::string getBasePath() const;

    /**
     * @brief Loads a texture from file and stores it with the given ID
     * @param renderer The SDL renderer to create the texture with
     * @param id Unique identifier for the texture
     * @param path File path to the image file
     * @return Pointer to the loaded SDL texture, or nullptr on failure
     */
    SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& id, const std::string& path);

    /**
     * @brief Returns the uniform scale factor applied to a texture at load time.
     * If a texture was downscaled to fit GPU limits, this value will be < 1.0.
     * If no scaling was applied, returns 1.0.
     */
    float getTextureScale(const std::string& id) const;

    /**
     * @brief Retrieves a previously loaded texture by ID
     * @param id The unique identifier of the texture
     * @return Pointer to the SDL texture, or nullptr if not found
     */
    SDL_Texture* getTexture(const std::string& id) const;

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
    // Tracks per-texture uniform downscale factor applied at load time
    std::unordered_map<std::string, float> textureScales;
    std::string basePath_;
    std::string defaultFontPath = "client/res/fonts/OpenSans-Medium.ttf";
    std::unordered_map<int, TTF_Font*> fonts;
};

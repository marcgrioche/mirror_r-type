/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** GraphicsManager - Singleton for SDL renderer management
*/

#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>

class GraphicsManager {
public:
    /**
     * @brief Gets the singleton instance of GraphicsManager
     * @return Reference to the singleton GraphicsManager instance
     */
    static GraphicsManager& getInstance();

    /**
     * @brief Initializes the SDL window and renderer
     * @param title Window title
     * @param width Window width in pixels
     * @param height Window height in pixels
     * @return True if initialization succeeded, false otherwise
     */
    bool initialize(const char* title, int width, int height);

    /**
     * @brief Cleans up SDL resources and shuts down graphics
     */
    void cleanup();

    /**
     * @brief Gets the SDL renderer
     * @return Pointer to the SDL renderer, or nullptr if not initialized
     */
    SDL_Renderer* getRenderer() const;

    /**
     * @brief Gets the SDL window
     * @return Pointer to the SDL window, or nullptr if not initialized
     */
    SDL_Window* getWindow() const;

    /**
     * @brief Clears the screen with the specified color
     * @param r Red component (0-255)
     * @param g Green component (0-255)
     * @param b Blue component (0-255)
     * @param a Alpha component (0-255, default 255)
     */
    void clear(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255);

    /**
     * @brief Presents the rendered frame to the screen
     */
    void present();

    /**
     * @brief Sets the drawing color for subsequent rendering operations
     * @param r Red component (0-255)
     * @param g Green component (0-255)
     * @param b Blue component (0-255)
     * @param a Alpha component (0-255, default 255)
     */
    void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

    /**
     * @brief Checks if the graphics manager is properly initialized
     * @return True if initialized, false otherwise
     */
    bool isInitialized() const;

    /**
     * @brief Gets the actual window dimensions
     * @param width Output parameter for window width
     * @param height Output parameter for window height
     */
    void getWindowSize(int& width, int& height) const;

    /**
     * @brief Toggles fullscreen mode on/off
     * @param fullscreen True to enable fullscreen desktop mode, false for windowed
     */
    void setFullscreen(bool fullscreen);

    /**
     * @brief Gets the logical resolution (game coordinates)
     * @param width Output parameter for logical width
     * @param height Output parameter for logical height
     */
    void getLogicalSize(int& width, int& height) const;

    /**
     * @brief Gets the render resolution (internal rendering quality)
     * @param width Output parameter for render width
     * @param height Output parameter for render height
     */
    void getRenderSize(int& width, int& height) const;

    /**
     * @brief Changes the window size dynamically
     * @param width New window width
     * @param height New window height
     */
    void setWindowSize(int width, int height);

    /**
     * @brief Changes the render resolution dynamically
     * @param width New render width
     * @param height New render height
     */
    void setRenderSize(int width, int height);

    /**
     * @brief Sets the colorblind filter type
     * @param type The colorblind filter type ("protanopia", "deuteranopia", "tritanopia", or empty for no filter)
     */
    void setColorblindMode(std::string type);

private:
    GraphicsManager() = default;
    ~GraphicsManager() = default;

    GraphicsManager(const GraphicsManager&) = delete;
    GraphicsManager& operator=(const GraphicsManager&) = delete;

    void applyScalingOptions();
    void createRenderTarget();
    void destroyRenderTarget();
    void applyHardwareColorblindFilter();
    void createColorblindFilterTextures();
    void destroyColorblindFilterTextures();

    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    SDL_Texture* m_renderTarget = nullptr;
    SDL_Texture* m_colorblindFilterTexture = nullptr;
    bool m_initialized = false;
    bool m_useRenderTarget = false;
    std::string m_colorblindType;

    struct ColorModulation {
        Uint8 r, g, b;
    } m_colorModulation = { 255, 255, 255 };

    int m_logicalWidth = 0;
    int m_logicalHeight = 0;
    int m_renderWidth = 0;
    int m_renderHeight = 0;
};

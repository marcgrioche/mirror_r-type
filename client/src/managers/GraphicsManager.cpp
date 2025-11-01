/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** GraphicsManager implementation
*/

#include "GraphicsManager.hpp"
#include "Config.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

GraphicsManager& GraphicsManager::getInstance()
{
    static GraphicsManager instance;
    return instance;
}

bool GraphicsManager::initialize(const char* title, int width, int height)
{
    if (m_initialized) {
        std::cerr << "GraphicsManager already initialized!" << std::endl;
        return true;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize. SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    Uint32 windowFlags = SDL_WINDOW_SHOWN;

    switch (DISPLAY_MODE) {
    case DisplayMode::FULLSCREEN_DESKTOP:
        windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        break;
    case DisplayMode::FULLSCREEN_EXCLUSIVE:
        windowFlags |= SDL_WINDOW_FULLSCREEN;
        break;
    case DisplayMode::WINDOWED:
        break;
    }

    if (DISPLAY_MODE == DisplayMode::WINDOWED) {
        windowFlags |= SDL_WINDOW_RESIZABLE;
    }

    m_window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width, height,
        windowFlags);

    if (m_window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    int actualWidth, actualHeight;
    SDL_GetWindowSize(m_window, &actualWidth, &actualHeight);

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    m_logicalWidth = GAME_LOGICAL_WIDTH;
    m_logicalHeight = GAME_LOGICAL_HEIGHT;
    m_renderWidth = RENDER_WIDTH;
    m_renderHeight = RENDER_HEIGHT;

    m_useRenderTarget = (m_renderWidth != m_logicalWidth || m_renderHeight != m_logicalHeight || !m_colorblindType.empty());

    if (m_useRenderTarget) {
        createRenderTarget();
    }

    SDL_RenderSetLogicalSize(m_renderer, m_logicalWidth, m_logicalHeight);
    applyScalingOptions();

    m_initialized = true;

    return true;
}

void GraphicsManager::cleanup()
{
    if (m_initialized) {
        destroyColorblindFilterTextures();
        destroyRenderTarget();
        if (m_renderer) {
            SDL_DestroyRenderer(m_renderer);
            m_renderer = nullptr;
        }
        if (m_window) {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }
        SDL_Quit();
        TTF_Quit();
        m_initialized = false;
    }
}

SDL_Renderer* GraphicsManager::getRenderer() const
{
    if (m_renderer && m_useRenderTarget && m_renderTarget) {
        SDL_SetRenderTarget(m_renderer, m_renderTarget);
    }
    return m_renderer;
}

SDL_Window* GraphicsManager::getWindow() const
{
    return m_window;
}

void GraphicsManager::clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    if (!m_renderer)
        return;

    if (m_useRenderTarget && m_renderTarget) {
        SDL_SetRenderTarget(m_renderer, m_renderTarget);
    }

    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
    SDL_RenderClear(m_renderer);
}

void GraphicsManager::present()
{
    if (!m_renderer)
        return;

    if (m_useRenderTarget && m_renderTarget) {
        SDL_SetRenderTarget(m_renderer, nullptr);

        if (!m_colorblindType.empty()) {
            applyHardwareColorblindFilter();
        } else {
            SDL_RenderCopy(m_renderer, m_renderTarget, nullptr, nullptr);
        }
    }

    SDL_RenderPresent(m_renderer);
}

void GraphicsManager::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    if (!m_renderer)
        return;

    if (m_useRenderTarget && m_renderTarget) {
        SDL_SetRenderTarget(m_renderer, m_renderTarget);
    }

    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

bool GraphicsManager::isInitialized() const
{
    return m_initialized;
}

void GraphicsManager::getWindowSize(int& width, int& height) const
{
    if (m_window) {
        SDL_GetWindowSize(m_window, &width, &height);
    } else {
        width = 0;
        height = 0;
    }
}

void GraphicsManager::setFullscreen(bool fullscreen)
{
    if (m_window) {
        Uint32 flags = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
        SDL_SetWindowFullscreen(m_window, flags);
    }
}

void GraphicsManager::getLogicalSize(int& width, int& height) const
{
    width = m_logicalWidth;
    height = m_logicalHeight;
}

void GraphicsManager::getRenderSize(int& width, int& height) const
{
    width = m_renderWidth;
    height = m_renderHeight;
}

void GraphicsManager::setWindowSize(int width, int height)
{
    if (m_window && DISPLAY_MODE == DisplayMode::WINDOWED) {
        SDL_SetWindowSize(m_window, width, height);
        SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }
}

void GraphicsManager::setRenderSize(int width, int height)
{
    if (!m_renderer)
        return;

    bool needsRenderTarget = (width != m_logicalWidth || height != m_logicalHeight);

    if (needsRenderTarget != m_useRenderTarget) {
        destroyRenderTarget();
        m_useRenderTarget = needsRenderTarget;
    }

    m_renderWidth = width;
    m_renderHeight = height;

    if (m_useRenderTarget) {
        destroyRenderTarget();
        createRenderTarget();
    }
}

void GraphicsManager::applyScalingOptions()
{
    if (!m_renderer)
        return;

    if (INTEGER_SCALING) {
        SDL_RenderSetIntegerScale(m_renderer, SDL_TRUE);
    } else {
        SDL_RenderSetIntegerScale(m_renderer, SDL_FALSE);
    }
}

void GraphicsManager::createRenderTarget()
{
    if (!m_renderer)
        return;

    m_renderTarget = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        m_renderWidth,
        m_renderHeight);

    if (!m_renderTarget) {
        std::cerr << "Failed to create render target: " << SDL_GetError() << std::endl;
        m_useRenderTarget = false;
    }
}

void GraphicsManager::destroyRenderTarget()
{
    if (m_renderTarget) {
        SDL_DestroyTexture(m_renderTarget);
        m_renderTarget = nullptr;
    }
}

void GraphicsManager::setColorblindMode(std::string type)
{
    m_colorblindType = type;

    if (type == "protanopia") {
        // Red-blind: reduce red channel, emphasize green
        m_colorModulation = { 145, 200, 255 }; // ~57% red, ~78% green, full blue
    } else if (type == "deuteranopia") {
        // Green-blind: reduce green channel, emphasize red
        m_colorModulation = { 200, 180, 255 }; // ~78% red, ~70% green, full blue
    } else if (type == "tritanopia") {
        // Blue-blind: reduce blue channel, emphasize yellow tones
        m_colorModulation = { 255, 220, 134 }; // full red, ~86% green, ~52% blue
    } else {
        // No filter - full color
        m_colorModulation = { 255, 255, 255 };
    }

    if (!type.empty()) {
        if (!m_renderTarget) {
            createRenderTarget();
        }
        if (m_renderTarget) {
            m_useRenderTarget = true;
        }
    } else {
        m_useRenderTarget = (m_renderWidth != m_logicalWidth || m_renderHeight != m_logicalHeight);
        if (!m_useRenderTarget) {
            destroyRenderTarget();
        }
    }
}

void GraphicsManager::applyHardwareColorblindFilter()
{
    if (!m_renderTarget || m_colorblindType.empty()) {
        SDL_RenderCopy(m_renderer, m_renderTarget, nullptr, nullptr);
        return;
    }

    SDL_SetTextureColorMod(m_renderTarget, m_colorModulation.r, m_colorModulation.g, m_colorModulation.b);
    SDL_RenderCopy(m_renderer, m_renderTarget, nullptr, nullptr);

    SDL_SetTextureColorMod(m_renderTarget, 255, 255, 255);
}

void GraphicsManager::createColorblindFilterTextures()
{
}

void GraphicsManager::destroyColorblindFilterTextures()
{
    if (m_colorblindFilterTexture) {
        SDL_DestroyTexture(m_colorblindFilterTexture);
        m_colorblindFilterTexture = nullptr;
    }
}

void GraphicsManager::windowToLogical(int windowX, int windowY, int& logicalX, int& logicalY) const
{
    if (!m_renderer || !m_window) {
        logicalX = windowX;
        logicalY = windowY;
        return;
    }

    // Get the actual window size
    int windowWidth, windowHeight;
    SDL_GetWindowSize(m_window, &windowWidth, &windowHeight);

    // Get the logical size
    int logicalWidth = m_logicalWidth;
    int logicalHeight = m_logicalHeight;

    // Calculate the scaling and viewport
    float scaleX = static_cast<float>(windowWidth) / static_cast<float>(logicalWidth);
    float scaleY = static_cast<float>(windowHeight) / static_cast<float>(logicalHeight);

    // When using SDL_RenderSetLogicalSize with letter/pillarboxing,
    // SDL maintains aspect ratio. We need to account for this.
    float scale;
    int viewportX = 0;
    int viewportY = 0;
    int viewportWidth = windowWidth;
    int viewportHeight = windowHeight;

    if (MAINTAIN_ASPECT_RATIO) {
        // SDL uses the smaller scale to maintain aspect ratio
        scale = (scaleX < scaleY) ? scaleX : scaleY;

        // Calculate viewport (the actual rendering area)
        viewportWidth = static_cast<int>(logicalWidth * scale);
        viewportHeight = static_cast<int>(logicalHeight * scale);

        // Calculate letterbox/pillarbox offsets
        viewportX = (windowWidth - viewportWidth) / 2;
        viewportY = (windowHeight - viewportHeight) / 2;
    } else {
        scale = scaleX; // Use X scale if not maintaining aspect ratio
    }

    // Convert window coordinates to logical coordinates
    // First, subtract the viewport offset (letterbox/pillarbox)
    int adjustedX = windowX - viewportX;
    int adjustedY = windowY - viewportY;

    // Then scale down to logical coordinates
    logicalX = static_cast<int>(adjustedX / scale);
    logicalY = static_cast<int>(adjustedY / scale);

    // Clamp to logical bounds (in case mouse is outside viewport)
    if (logicalX < 0)
        logicalX = 0;
    if (logicalY < 0)
        logicalY = 0;
    if (logicalX >= logicalWidth)
        logicalX = logicalWidth - 1;
    if (logicalY >= logicalHeight)
        logicalY = logicalHeight - 1;
}

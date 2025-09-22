/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** GraphicsManager implementation
*/

#include "GraphicsManager.hpp"
#include <iostream>

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

    m_window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN);

    if (m_window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    m_initialized = true;
    std::cout << "GraphicsManager initialized successfully!" << std::endl;
    return true;
}

void GraphicsManager::cleanup()
{
    if (m_initialized) {
        if (m_renderer) {
            SDL_DestroyRenderer(m_renderer);
            m_renderer = nullptr;
        }
        if (m_window) {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }
        SDL_Quit();
        m_initialized = false;
        std::cout << "GraphicsManager cleaned up." << std::endl;
    }
}

SDL_Renderer* GraphicsManager::getRenderer() const
{
    return m_renderer;
}

SDL_Window* GraphicsManager::getWindow() const
{
    return m_window;
}

void GraphicsManager::clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    if (m_renderer) {
        SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
        SDL_RenderClear(m_renderer);
    }
}

void GraphicsManager::present()
{
    if (m_renderer) {
        SDL_RenderPresent(m_renderer);
    }
}

void GraphicsManager::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    if (m_renderer) {
        SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
    }
}

bool GraphicsManager::isInitialized() const
{
    return m_initialized;
}

/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** GraphicsManager - Singleton for SDL renderer management
*/

#pragma once
#include <SDL2/SDL.h>
#include <memory>

class GraphicsManager {
public:
    // Get the singleton instance
    static GraphicsManager& getInstance();

    bool initialize(const char* title, int width, int height);

    void cleanup();

    SDL_Renderer* getRenderer() const;
    SDL_Window* getWindow() const;

    void clear(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255);
    void present();
    void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

    bool isInitialized() const;

private:
    GraphicsManager() = default;
    ~GraphicsManager() = default;

    GraphicsManager(const GraphicsManager&) = delete;
    GraphicsManager& operator=(const GraphicsManager&) = delete;

    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    bool m_initialized = false;
};

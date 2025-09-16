/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** main
*/

#include "ecs/Components.hpp"
#include "ecs/Registry.hpp"
#include <SDL2/SDL.h>
#include <chrono>
#include <iostream>

// Render everything
void renderSystem(Registry& registry, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 20, 30, 50, 255);
    SDL_RenderClear(renderer);

    auto& ps = registry.get_or_create_storage<Position>();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (size_t i = 0; i < ps.dense_size(); ++i) {
        auto pos = ps.get_by_dense_index(i);

        SDL_Rect rect = {
            static_cast<int>(pos.x * 20 + 100),
            static_cast<int>(pos.y * 20 + 100),
            15, 15
        };

        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
}

// Move everything
void movementSystem(Registry& registry, float deltaTime)
{
    auto view = registry.view<Position, Velocity>();
    for (auto&& [pos, vel] : view) {
        pos.x += vel.dx * deltaTime;
        pos.y += vel.dy * deltaTime;
    }
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize. SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("R-Type - ECS + SDL2 Demo",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::cout << "SDL2 + ECS Demo initialized successfully!" << std::endl;
    std::cout << "Watch the moving white squares. Press ESC or close window to exit." << std::endl;

    Registry registry;

    Entity e1 = registry.create_entity();
    Entity e2 = registry.create_entity();
    Entity e3 = registry.create_entity();

    registry.emplace<Position>(e1, 0.0f, 0.0f);
    registry.emplace<Velocity>(e1, 50.0f, 30.0f); // pixels per second

    registry.emplace<Position>(e2, 10.0f, 5.0f);
    registry.emplace<Velocity>(e2, -20.0f, 40.0f);

    registry.emplace<Position>(e3, 5.0f, 10.0f);
    registry.emplace<Velocity>(e3, 30.0f, -25.0f);

    bool quit = false;
    SDL_Event e;
    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!quit) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                } else if (e.key.keysym.sym == SDLK_SPACE) {
                    Entity newEntity = registry.create_entity();
                    registry.emplace<Position>(newEntity,
                        static_cast<float>(rand() % 20),
                        static_cast<float>(rand() % 15));
                    registry.emplace<Velocity>(newEntity,
                        static_cast<float>((rand() % 100) - 50),
                        static_cast<float>((rand() % 100) - 50));
                    std::cout << "Created new entity: " << newEntity << std::endl;
                }
            }
        }

        movementSystem(registry, deltaTime);

        auto& ps = registry.get_or_create_storage<Position>();
        for (size_t i = 0; i < ps.dense_size(); ++i) {
            auto& pos = ps.get_by_dense_index(i);
            if (pos.x < -5)
                pos.x = 35;
            if (pos.x > 35)
                pos.x = -5;
            if (pos.y < -5)
                pos.y = 25;
            if (pos.y > 25)
                pos.y = -5;
        }

        renderSystem(registry, renderer);

        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "Demo completed successfully!" << std::endl;
    return 0;
}

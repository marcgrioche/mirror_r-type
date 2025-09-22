/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** main
*/

#include "GraphicsManager.hpp"
#include "ecs/Components.hpp"
#include "ecs/Registry.hpp"
#include "ecs/systems/MovementSystem.hpp"
#include "ecs/systems/RenderSystem.hpp"
#include <SDL2/SDL.h>
#include <chrono>
#include <iostream>

int main()
{
    // Initialize graphics with the singleton pattern
    auto& graphics = GraphicsManager::getInstance();
    if (!graphics.initialize("R-Type - ECS + SDL2 Demo", 800, 600)) {
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
        boundarySystem(registry);
        renderSystem(registry);

        SDL_Delay(16); // ~60 FPS
    }

    graphics.cleanup();

    std::cout << "Demo completed successfully!" << std::endl;
    return 0;
}

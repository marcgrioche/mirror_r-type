/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** MovementSystem implementation
*/

#include "MovementSystem.hpp"
#include "components/AllComponents.hpp"
#include <random>
#include <array>

void movementPlatform(Registry& registry, float deltaTime)
{
    constexpr float ScreenW   = 800.0f;
    constexpr float PlatformW = 120.0f;

    std::array<float, 4> levels = {200.0f, 300.0f, 400.0f, 500.0f};


    constexpr float minDistX   = 200.0f;
    constexpr float maxDistX   = 270.0f;
    constexpr float crossDistX = PlatformW * 1.5f;
    constexpr float maxDiffY   = 100.0f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distLevel(0, static_cast<int>(levels.size() - 1));

    // Récupère toutes les plateformes actuelles
    struct Plat { float x, y; };
    std::vector<Plat> platforms;
    {
        auto view = registry.view<Position, PlatformTag>();
        for (auto&& [pos, tag] : view) {
            platforms.push_back({pos.x, pos.y});
        }
    }

    // Bouge les plateformes
    auto view = registry.view<Position, Velocity, PlatformTag>();
    for (auto&& [pos, vel, tag] : view) {
        pos.x += vel.dx * deltaTime;
        pos.y += vel.dy * deltaTime;

        if (pos.x < -PlatformW) {
            float newX, newY;
            bool valid = false;

            for (int tries = 0; tries < 50 && !valid; ++tries) {
                newX = ScreenW;
                newY = levels[distLevel(gen)];
                valid = true;

                for (auto& p : platforms) {
                    if (p.y == newY && std::fabs(newX - p.x) < minDistX) {
                        valid = false;
                        break;
                    }
                    if (std::fabs(p.y - newY) <= maxDiffY && std::fabs(newX - p.x) < crossDistX) {
                        valid = false;
                        break;
                    }
                }
                if (!valid) continue;

                bool reachable = platforms.empty();
                for (auto& p : platforms) {
                    if (std::fabs(newX - p.x) <= maxDistX &&
                        std::fabs(p.y - newY) <= maxDiffY) {
                        reachable = true;
                        break;
                    }
                }

                if (!reachable) valid = false;
            }

            // Place la plateforme
            pos.x = newX;
            pos.y = newY;
            platforms.push_back({newX, newY});
        }
    }
}

void movementSystem(Registry& registry, float deltaTime)
{
    auto prevPosView = registry.view<Position, PreviousPosition>();
    for (auto&& [pos, prevPos] : prevPosView) {
        prevPos.x = pos.x;
        prevPos.y = pos.y;
    }

    auto view = registry.view<Position, Velocity, PlayerTag>();
    for (auto&& [pos, vel, tag] : view) {
        pos.x += vel.dx * deltaTime;
        pos.y += vel.dy * deltaTime;

        if (pos.x < 0) {
            pos.x = 0;
            vel.dx = 0;
        }
        if (pos.x > 800 - 32) {
            pos.x = 800 - 32;
            vel.dx = 0;
        }
    }
    movementPlatform(registry, deltaTime);
}

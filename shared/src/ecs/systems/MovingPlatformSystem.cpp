#include "MovingPlatformSystem.hpp"

constexpr float SCREEN_WIDTH = 800.0f;

void movingPlatformSystem(Registry& registry, float deltaTime)
{
    auto view = registry.view<MovingPlatform, Position>();
    for (auto&& [platform, pos] : view) {
        pos.x -= platform.speed * deltaTime;

        // Si la plateforme quitte l'écran à gauche, on la replace à droite
        if (pos.x + 120 < 0) { // 120 = largeur de la plateforme
            pos.x = SCREEN_WIDTH;
        }
    }
}
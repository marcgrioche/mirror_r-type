#include "CreatePlatform.hpp"
#include "Hitbox.hpp"
#include "Platform.hpp"
#include "Position.hpp"
#include "Tags.hpp"
#include "Velocity.hpp"
#include <array>
#include <random>

Entity factories::createOneWayPlatform(Registry& registry, float posx, float posy)
{
    Entity platform = registry.create_entity();
    registry.emplace<Position>(platform, Position { posx, posy });
    registry.emplace<Velocity>(platform, -10.0f, 0.0f);
    registry.emplace<Hitbox>(platform, Hitbox { 120, 20, 0, 0 });
    registry.emplace<PlatformTag>(platform);
    registry.emplace<BottomPassPlatform>(platform);
    return platform;
}

Entity factories::createPlatform(Registry& registry, float posx, float posy)
{
    Entity platform = registry.create_entity();
    registry.emplace<Position>(platform, Position { posx, posy });
    registry.emplace<Velocity>(platform, -10.0f, 0.0f);
    registry.emplace<Hitbox>(platform, Hitbox { 120, 20, 0, 0 });
    registry.emplace<PlatformTag>(platform);
    registry.emplace<NoPassPlatform>(platform);
    return platform;
}

std::vector<Entity> factories::generateRandomPlatforms(Registry& registry, int quantity)
{
    std::vector<Entity> PlatformsEntities;
    constexpr float ScreenW = 800.0f + 200.0f;
    constexpr float PlatformW = 120.0f;

    std::array<float, 4> levels = { 200.0f, 300.0f, 400.0f, 500.0f };

    constexpr float minDistX = 200.0f; // distance minimale sur le même étage
    constexpr float maxDistX = 270.0f; // distance maximale pour être atteignable
    constexpr float crossDistX = PlatformW * 1.5f; // distance minimale entre étages voisins
    constexpr float maxDiffY = 100.0f; // écart vertical autorisé pour sauter étage ±1

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(0.0f, ScreenW - PlatformW);
    std::uniform_int_distribution<int> distLevel(0, static_cast<int>(levels.size() - 1));

    struct Plat {
        float x, y;
    };
    std::vector<Plat> platforms;

    for (int i = 0; i < quantity; ++i) {
        float x, y;
        bool valid = false;

        for (int tries = 0; tries < 50 && !valid; ++tries) {
            x = distX(gen);
            y = levels[distLevel(gen)];
            valid = true;

            // Vérifie distance minimale avec toutes les plateformes existantes
            for (auto& p : platforms) {
                // Même étage → minDistX
                if (p.y == y && std::fabs(x - p.x) < minDistX) {
                    valid = false;
                    break;
                }
                // Étages voisins (±1) → crossDistX
                if (std::fabs(p.y - y) <= maxDiffY && std::fabs(x - p.x) < crossDistX) {
                    valid = false;
                    break;
                }
            }

            if (!valid)
                continue;

            // Vérifie qu’au moins une plateforme est atteignable
            bool reachable = platforms.empty(); // la première est toujours acceptée
            for (auto& p : platforms) {
                if (std::fabs(x - p.x) <= maxDistX && std::fabs(p.y - y) <= maxDiffY) {
                    reachable = true;
                    break;
                }
            }

            if (!reachable)
                valid = false;
        }

        platforms.push_back({ x, y });
        PlatformsEntities.push_back(createOneWayPlatform(registry, x, y));
    }
    return PlatformsEntities;
}

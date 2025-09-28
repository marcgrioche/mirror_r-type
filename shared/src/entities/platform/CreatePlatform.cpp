#include "CreatePlatform.hpp"
#include "Hitbox.hpp"
#include "Velocity.hpp"
#include "Platform.hpp"
#include "Position.hpp"
#include "Tags.hpp"
#include <random>

Entity factories::createOneWayPlatform(Registry& registry, float posx, float posy)
{
    Entity platform = registry.create_entity();
    registry.emplace<Position>(platform, Position { posx, posy });
    registry.emplace<Hitbox>(platform, Hitbox { 120, 20, 0, 0 });
    registry.emplace<PlatformTag>(platform);
    registry.emplace<BottomPassPlatform>(platform);
    return platform;
}

Entity factories::createPlatform(Registry& registry, float posx, float posy)
{
    Entity platform = registry.create_entity();
    registry.emplace<Position>(platform, Position { posx, posy });
    registry.emplace<Velocity>(platform, 0.0f, 0.0f);
    registry.emplace<Hitbox>(platform, Hitbox { 120, 20, 0, 0 });
    registry.emplace<PlatformTag>(platform);
    registry.emplace<NoPassPlatform>(platform);
    return platform;
}

void factories::generateRandomPlatforms(Registry& registry, int quantity)
{
    constexpr float ScreenW = 800.0f;
    constexpr float ScreenH = 600.0f;
    constexpr float PlatformW = 120.0f;
    constexpr float PlatformH = 20.0f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(0.0f, ScreenW - PlatformW);
    std::uniform_real_distribution<float> distY(200.0f, ScreenH - PlatformH);

    for (int i = 0; i < quantity; ++i) {
        float x = distX(gen);
        float y = distY(gen);
        createPlatform(registry, x, y);
    }
}

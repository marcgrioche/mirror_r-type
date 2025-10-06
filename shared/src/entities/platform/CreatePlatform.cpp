#include "CreatePlatform.hpp"
#include "Hitbox.hpp"
#include "Platform.hpp"
#include "Position.hpp"
#include "Tags.hpp"
#include "Dead.hpp"
#include "Velocity.hpp"
#include <array>
#include <random>

Entity factories::createOneWayPlatform(Registry& registry, float posx, float posy)
{
    Entity platform = registry.create_entity();
    registry.emplace<Position>(platform, Position { posx, posy });
    registry.emplace<Velocity>(platform, -50.0f, 0.0f);
    registry.emplace<Hitbox>(platform, Hitbox { 120, 20, 0, 0 });
    registry.emplace<PlatformTag>(platform);
    registry.emplace<BottomPassPlatform>(platform);
    registry.emplace<Dead>(platform);
    return platform;
}

Entity factories::createPlatform(Registry& registry, float posx, float posy)
{
    Entity platform = registry.create_entity();
    registry.emplace<Position>(platform, Position { posx, posy });
    registry.emplace<Velocity>(platform, -50.0f, 0.0f);
    registry.emplace<Hitbox>(platform, Hitbox { 120, 20, 0, 0 });
    registry.emplace<PlatformTag>(platform);
    registry.emplace<NoPassPlatform>(platform);
    registry.emplace<Dead>(platform);
    return platform;
}

std::vector<Entity> factories::generateRandomPlatforms(Registry& registry, int quantity)
{
    std::vector<Entity> platformsEntities;
    constexpr float SCREEN_W     = 800.0f + 200.0f;   // on génère un peu plus loin que l'écran initial
    constexpr float PLATFORM_W   = 120.0f;
    constexpr float MIN_Y        = 180.0f;            // bande jouable haute
    constexpr float MAX_Y        = 540.0f;            // bande jouable basse
    constexpr float SAME_LVL_EPS = 28.0f;             // seuil pour considérer même "étage"
    constexpr float MIN_DIST_X_SAME = 200.0f;         // distance minimale sur même étage
    constexpr float CROSS_DIST_X    = PLATFORM_W * 1.5f; // distance min entre étages proches
    constexpr float MAX_DIFF_Y      = 110.0f;         // delta Y maximal pour un saut atteignable
    constexpr float REACHABLE_X     = 270.0f;         // distance horizontale max atteignable

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(0.0f, SCREEN_W - PLATFORM_W);
    std::uniform_real_distribution<float> distY(MIN_Y, MAX_Y);

    struct Plat { float x, y; };
    std::vector<Plat> placed; placed.reserve(quantity);

    int topCount = 0, bottomCount = 0;
    const int edgeQuota = std::max(1, quantity / 3); // limiter empilement extrême haut/bas

    for (int i = 0; i < quantity; ++i) {
        float x = 0.f, y = 0.f;
        bool accepted = false;
        for (int attempt = 0; attempt < 80 && !accepted; ++attempt) {
            x = distX(gen);
            y = distY(gen);

            bool isTopBand    = y < (MIN_Y + (MAX_Y - MIN_Y) * 0.18f);
            bool isBottomBand = y > (MIN_Y + (MAX_Y - MIN_Y) * 0.82f);
            if ((isTopBand && topCount >= edgeQuota) || (isBottomBand && bottomCount >= edgeQuota))
                continue; // trop d'accumulation extrême

            bool okDistances = true;
            for (auto& p : placed) {
                float dx = std::fabs(x - p.x);
                float dy = std::fabs(y - p.y);
                if (dy < SAME_LVL_EPS && dx < MIN_DIST_X_SAME) { okDistances = false; break; }
                if (dy <= MAX_DIFF_Y && dx < CROSS_DIST_X) { okDistances = false; break; }
            }
            if (!okDistances) continue;

            bool reachable = placed.empty();
            if (!reachable) {
                for (auto& p : placed) {
                    float dx = std::fabs(x - p.x);
                    float dy = std::fabs(y - p.y);
                    if (dx <= REACHABLE_X && dy <= MAX_DIFF_Y) { reachable = true; break; }
                }
            }
            if (!reachable) continue;

            // accepté
            accepted = true;
            if (isTopBand) ++topCount; else if (isBottomBand) ++bottomCount;
        }

        placed.push_back({x, y});
        platformsEntities.push_back(createOneWayPlatform(registry, x, y));
    }

    return platformsEntities;
}

std::vector<Entity> factories::reGenerateRandomPlatforms(Registry& registry, int quantity)
{
    std::vector<Entity> platformsEntities;
    constexpr float SCREEN_W     = 800.0f;
    constexpr float PLATFORM_W   = 120.0f;
    constexpr float SPAWN_OFFSET = 80.0f;            // démarre juste hors écran
    constexpr float MIN_Y        = 180.0f;
    constexpr float MAX_Y        = 540.0f;
    constexpr float SAME_LVL_EPS = 28.0f;
    constexpr float MIN_DIST_X_SAME = 200.0f;
    constexpr float CROSS_DIST_X    = PLATFORM_W * 1.5f;
    constexpr float MAX_DIFF_Y      = 110.0f;
    constexpr float REACHABLE_X     = 270.0f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(SCREEN_W + SPAWN_OFFSET, SCREEN_W + SPAWN_OFFSET);
    std::uniform_real_distribution<float> distY(MIN_Y, MAX_Y);

    struct Plat { float x, y; };
    std::vector<Plat> placed; // On récupère les plateformes déjà existantes pour cohérence
    {
        auto view = registry.view<Position, PlatformTag>();
        for (auto&& [pos, tag] : view) {
            placed.push_back({pos.x, pos.y});
        }
    }

    int topCount = 0, bottomCount = 0;
    const int edgeQuota = std::max(1, quantity / 3);

    for (int i = 0; i < quantity; ++i) {
        float x = 0.f, y = 0.f;
        bool accepted = false;
        for (int attempt = 0; attempt < 80 && !accepted; ++attempt) {
            x = distX(gen);
            y = distY(gen);

            bool isTopBand    = y < (MIN_Y + (MAX_Y - MIN_Y) * 0.18f);
            bool isBottomBand = y > (MIN_Y + (MAX_Y - MIN_Y) * 0.82f);
            if ((isTopBand && topCount >= edgeQuota) || (isBottomBand && bottomCount >= edgeQuota))
                continue;

            bool okDistances = true;
            for (auto& p : placed) {
                float dx = std::fabs(x - p.x);
                float dy = std::fabs(y - p.y);
                if (dy < SAME_LVL_EPS && dx < MIN_DIST_X_SAME) { okDistances = false; break; }
                if (dy <= MAX_DIFF_Y && dx < CROSS_DIST_X) { okDistances = false; break; }
            }
            if (!okDistances) continue;

            bool reachable = placed.empty();
            if (!reachable) {
                for (auto& p : placed) {
                    float dx = std::fabs(x - p.x);
                    float dy = std::fabs(y - p.y);
                    if (dx <= REACHABLE_X && dy <= MAX_DIFF_Y) { reachable = true; break; }
                }
            }
            if (!reachable) continue;

            accepted = true;
            if (isTopBand) ++topCount; else if (isBottomBand) ++bottomCount;
        }

        placed.push_back({x, y});
        platformsEntities.push_back(createOneWayPlatform(registry, x, y));
    }
    return platformsEntities;
}

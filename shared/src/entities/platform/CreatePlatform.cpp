#include "CreatePlatform.hpp"
#include "../../levels/Level.hpp"
#include "Config.hpp"
#include <cmath>

Entity factories::createOneWayPlatform(Registry& registry, float posx, float posy, float velx, float vely, const Level* level)
{
    // If a level is provided, apply the level's size_factor to the platform hitbox
    float width = level ? (level->getPlatformWidth() * level->getPlatformSizeFactor()) : 100.0;
    float height = level ? (level->getPlatformHeight() * level->getPlatformSizeFactor()) : 100.0;

    std::cout << "[CREATE PLATFORM] sizefactor = " << level->getPlatformSizeFactor() << std::endl;
    if (level->getPlatformSizeFactor() < 1.0) {
        std::cout << "[CREATE PLATFORM 2222] sizefactor = " << level->getPlatformSizeFactor() << std::endl;
        width *= level->getPlatformSizeFactor();
        height *= level->getPlatformSizeFactor();
    }

    Entity platform = registry.create_entity();
    registry.emplace<Position>(platform, Position { posx, posy });
    registry.emplace<Velocity>(
        platform,
        velx,
        vely);
    registry.emplace<Hitbox>(platform, Hitbox { width, height, 0, 0 });
    registry.emplace<PlatformTag>(platform);
    registry.emplace<BottomPassPlatform>(platform);
    registry.emplace<Dead>(platform);
    return platform;
}

Entity factories::createOneWayPlatform(Registry& registry, float posx, float posy, float velx, float vely, float width, float height)
{
    Entity platform = registry.create_entity();
    registry.emplace<Position>(platform, Position { posx, posy });
    registry.emplace<Velocity>(
        platform,
        velx,
        vely);
    registry.emplace<Hitbox>(platform, Hitbox { width, height, 0, 0 });
    registry.emplace<PlatformTag>(platform);
    registry.emplace<BottomPassPlatform>(platform);
    registry.emplace<Dead>(platform);
    return platform;
}

Entity factories::createOneWayPlatform(Registry& registry, float posx, float posy, float velx, float vely, float width, float height, float offsetX, float offsetY)
{
    Entity platform = registry.create_entity();
    registry.emplace<Position>(platform, Position { posx, posy });
    registry.emplace<Velocity>(platform, velx, vely);
    registry.emplace<Hitbox>(platform, Hitbox { width, height, offsetX, offsetY });
    registry.emplace<PlatformTag>(platform);
    registry.emplace<BottomPassPlatform>(platform);
    registry.emplace<Dead>(platform);
    return platform;
}

Entity factories::createPlatform(Registry& registry, float posx, float posy, float velx, float vely, const Level* level)
{
    // Apply size_factor from level to ensure hitbox matches visual sprite when size_factor != 1
    float width = level ? level->getPlatformWidth(): 100.0;
    float height = level ? level->getPlatformHeight() : 100.0;

    Entity platform = registry.create_entity();
    registry.emplace<Position>(platform, Position { posx, posy });
    registry.emplace<Velocity>(
        platform,
        velx,
        vely);
    registry.emplace<Hitbox>(platform, Hitbox {  width, height, 0, 0 });
    registry.emplace<PlatformTag>(platform);
    registry.emplace<NoPassPlatform>(platform);
    registry.emplace<Dead>(platform);
    return platform;
}

std::vector<Entity> factories::generateRandomPlatforms(Registry& registry, int quantity, const Level* level)
{
    std::vector<Entity> platformsEntities;
    constexpr float SCREEN_W = SCREEN_WIDTH + 200.0f; // on génère un peu plus loin que l'écran initial
    // Use level-configured platform dimensions when available
    const float platformWidth = level ? level->getPlatformWidth() : PLATFORM_WIDTH;
    const float platformHeight = level ? level->getPlatformHeight() : PLATFORM_HEIGHT;
    // Use level parameters if provided, otherwise use defaults
    const float TOP_MARGIN = level ? level->getPlatformTopMargin() : 80.0f;
    const float BOTTOM_MARGIN = level ? level->getPlatformBottomMargin() : 100.0f;
    const float MIN_Y = TOP_MARGIN; // bande jouable haute
    const float MAX_Y = SCREEN_HEIGHT - BOTTOM_MARGIN - platformHeight; // bande jouable basse
    const float SAME_LVL_EPS = level ? level->getPlatformSameLevelEpsilon() : 28.0f; // seuil pour considérer même "étage"
    const float MIN_DIST_X_SAME = level ? level->getPlatformMinDistXSame() : 200.0f; // distance minimale sur même étage
    const float CROSS_DIST_X = level ? level->getPlatformCrossDistX() : 180.0f; // distance min entre étages proches
    const float MAX_DIFF_Y = level ? level->getPlatformMaxDiffY() : 110.0f; // delta Y maximal pour un saut atteignable
    const float REACHABLE_X = level ? level->getPlatformReachableX() : 270.0f; // distance horizontale max atteignable

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(0.0f, SCREEN_W - platformWidth);

    const int LAYER_COUNT = level ? level->getPlatformLayers() : 8;
    const float availableSpan = (MAX_Y - MIN_Y);
    const float targetSpacing = (std::min)(availableSpan / (LAYER_COUNT - 1), MAX_DIFF_Y - 5.f); // petite marge sécurité
    const float usedSpan = targetSpacing * (LAYER_COUNT - 1);
    const float verticalOffset = MIN_Y + (availableSpan - usedSpan) * 0.5f; // centre les layers dans la fenêtre jouable
    std::vector<float> layers(LAYER_COUNT);
    for (int i = 0; i < LAYER_COUNT; ++i)
        layers[i] = verticalOffset + targetSpacing * i; // valeurs exactes sans jitter
    std::uniform_int_distribution<int> pickLayer(0, LAYER_COUNT - 1);

    const float PLATFORM_VEL_X = level ? level->getPlatformVelocityX() : static_cast<float>(PLATFORM_VELOCITY_X);
    const float PLATFORM_VEL_Y = level ? level->getPlatformVelocityY() : static_cast<float>(PLATFORM_VELOCITY_Y);

    struct Plat {
        float x, y;
    };
    std::vector<Plat> placed;
    placed.reserve(quantity);

    int topCount = 0, bottomCount = 0;
    const int edgeQuota = std::max(1, quantity / 3); // limiter empilement extrême haut/bas

    placed.push_back({ 0.0f, SCREEN_HEIGHT / 2 });
    platformsEntities.push_back(createOneWayPlatform(registry, 0.0f, SCREEN_HEIGHT / 2, PLATFORM_VEL_X, PLATFORM_VEL_Y, level));

    for (int i = 0; i < quantity; ++i) {
        float x = 0.f, y = 0.f;
        bool accepted = false;
        for (int attempt = 0; attempt < 80 && !accepted; ++attempt) {
            x = distX(gen);
            y = layers[pickLayer(gen)];

            // On considère la première et dernière couche comme bandes extrêmes
            bool isTopBand = (y == layers.front());
            bool isBottomBand = (y == layers.back());
            if ((isTopBand && topCount >= edgeQuota) || (isBottomBand && bottomCount >= edgeQuota))
                continue; // trop d'accumulation extrême

            bool okDistances = true;
            for (auto& p : placed) {
                float dx = std::fabs(x - p.x);
                float dy = std::fabs(y - p.y);
                if (dy < SAME_LVL_EPS && dx < MIN_DIST_X_SAME) {
                    okDistances = false;
                    break;
                }
                if (dy <= MAX_DIFF_Y && dx < CROSS_DIST_X) {
                    okDistances = false;
                    break;
                }
            }
            if (!okDistances)
                continue;

            bool reachable = placed.empty();
            if (!reachable) {
                for (auto& p : placed) {
                    float dx = std::fabs(x - p.x);
                    float dy = std::fabs(y - p.y);
                    if (dx <= REACHABLE_X && dy <= MAX_DIFF_Y) {
                        reachable = true;
                        break;
                    }
                }
            }
            if (!reachable)
                continue;

            // accepté
            accepted = true;
            if (isTopBand)
                ++topCount;
            else if (isBottomBand)
                ++bottomCount;
        }

        placed.push_back({ x, y });
        platformsEntities.push_back(createOneWayPlatform(registry, x, y, PLATFORM_VEL_X, PLATFORM_VEL_Y, level));
    }

    return platformsEntities;
}

std::vector<Entity> factories::reGenerateRandomPlatforms(Registry& registry, int quantity, const Level* level)
{
    std::vector<Entity> platformsEntities;
    constexpr float SCREEN_W = SCREEN_WIDTH;
    constexpr float SPAWN_OFFSET = 80.0f; // démarre juste hors écran
    const float platformWidth = level ? level->getPlatformWidth() : PLATFORM_WIDTH;
    const float platformHeight = level ? level->getPlatformHeight() : PLATFORM_HEIGHT;
    // Use level parameters for platform regeneration if provided, otherwise use defaults
    const float TOP_MARGIN = level ? level->getPlatformTopMargin() : 80.0f;
    const float BOTTOM_MARGIN = level ? level->getPlatformBottomMargin() : 100.0f;
    const float MIN_Y = TOP_MARGIN;
    const float MAX_Y = SCREEN_HEIGHT - BOTTOM_MARGIN - platformHeight;
    const float SAME_LVL_EPS = level ? level->getPlatformSameLevelEpsilon() : 28.0f;
    const float MIN_DIST_X_SAME = level ? level->getPlatformMinDistXSame() : 200.0f;
    const float CROSS_DIST_X = level ? level->getPlatformCrossDistX() : 180.0f;
    const float MAX_DIFF_Y = level ? level->getPlatformMaxDiffY() : 110.0f;
    const float REACHABLE_X = level ? level->getPlatformReachableX() : 270.0f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(SCREEN_W, SCREEN_W + SPAWN_OFFSET);

    // Même logique de layers que dans generateRandomPlatforms
    const int LAYER_COUNT = level ? level->getPlatformLayers() : 8;
    const float availableSpan = (MAX_Y - MIN_Y);
    const float targetSpacing = (std::min)(availableSpan / (LAYER_COUNT - 1), MAX_DIFF_Y - 5.f); // petite marge sécurité
    const float usedSpan = targetSpacing * (LAYER_COUNT - 1);
    const float verticalOffset = MIN_Y + (availableSpan - usedSpan) * 0.5f; // centre les layers dans la fenêtre jouable
    std::vector<float> layers(LAYER_COUNT);
    for (int i = 0; i < LAYER_COUNT; ++i)
        layers[i] = verticalOffset + targetSpacing * i; // valeurs exactes sans jitter
    std::uniform_int_distribution<int> pickLayer(0, LAYER_COUNT - 1);

    const float PLATFORM_VEL_X = level ? level->getPlatformVelocityX() : static_cast<float>(PLATFORM_VELOCITY_X);
    const float PLATFORM_VEL_Y = level ? level->getPlatformVelocityY() : static_cast<float>(PLATFORM_VELOCITY_Y);

    struct Plat {
        float x, y;
    };
    std::vector<Plat> placed; // On récupère les plateformes déjà existantes pour cohérence
    {
        auto view = registry.view<Position, PlatformTag>();
        for (auto&& [pos, tag] : view) {
            placed.push_back({ pos.v.x, pos.v.y });
        }
    }

    int topCount = 0, bottomCount = 0;
    const int edgeQuota = std::max(1, quantity / 3);

    for (int i = 0; i < quantity; ++i) {
        float x = 0.f, y = 0.f;
        bool accepted = false;
        for (int attempt = 0; attempt < 80 && !accepted; ++attempt) {
            x = distX(gen);
            y = layers[pickLayer(gen)];

            bool isTopBand = (y == layers.front());
            bool isBottomBand = (y == layers.back());
            if ((isTopBand && topCount >= edgeQuota) || (isBottomBand && bottomCount >= edgeQuota))
                continue;

            bool okDistances = true;
            for (auto& p : placed) {
                float dx = std::fabs(x - p.x);
                float dy = std::fabs(y - p.y);
                if (dy < SAME_LVL_EPS && dx < MIN_DIST_X_SAME) {
                    okDistances = false;
                    break;
                }
                if (dy <= MAX_DIFF_Y && dx < CROSS_DIST_X) {
                    okDistances = false;
                    break;
                }
            }
            if (!okDistances)
                continue;

            bool reachable = placed.empty();
            if (!reachable) {
                for (auto& p : placed) {
                    float dx = std::fabs(x - p.x);
                    float dy = std::fabs(y - p.y);
                    if (dx <= REACHABLE_X && dy <= MAX_DIFF_Y) {
                        reachable = true;
                        break;
                    }
                }
            }
            if (!reachable)
                continue;

            accepted = true;
            if (isTopBand)
                ++topCount;
            else if (isBottomBand)
                ++bottomCount;
        }

        placed.push_back({ x, y });
        platformsEntities.push_back(createOneWayPlatform(registry, x, y, PLATFORM_VEL_X, PLATFORM_VEL_Y, level));
    }
    return platformsEntities;
}

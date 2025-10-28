/*
** BackgroundMenu.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 21 1:07:22 AM 2025 jojo
** Last update Wed Oct 28 4:17:03 PM 2025 jojo
*/

#include "BackgroundMenu.hpp"
#include "DisplayPresets.hpp"
#include "components/Position.hpp"
#include "components/Sprite.hpp"
#include "ecs/components/SpriteFactory.hpp"
#include "entities/eyes/CreateEye.hpp"
#include "render/EyeRender.hpp"
#include "render/SpriteRender.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

BackgroundMenu::BackgroundMenu()
{
}

BackgroundMenu::~BackgroundMenu()
{
    // nothing to do here: caller should call destroy(registry)
}

namespace {
// simple AABB overlap test with padding
bool rectsOverlap(float ax, float ay, float aw, float ah,
    float bx, float by, float bw, float bh, float padding = 4.0f)
{
    return !(ax + aw + padding <= bx || bx + bw + padding <= ax || ay + ah + padding <= by || by + bh + padding <= ay);
}
}

void BackgroundMenu::destroy(Registry& registry)
{
    // Kill background
    if (m_background.id != 0) {
        registry.kill_entity(m_background);
        m_background.id = 0;
    }

    // Kill all eyes (parent entity + children will be cleaned by factory or explicit kills)
    for (Entity e : listEye) {
        if (e.id != 0) {
            // try to remove pupil/outline if present
            if (registry.has<Eye>(e)) {
                Eye& eye = registry.get<Eye>(e);
                if (eye.pupilEntity.id != 0)
                    registry.kill_entity(eye.pupilEntity);
                if (eye.outlineEntity.id != 0)
                    registry.kill_entity(eye.outlineEntity);
            }
            registry.kill_entity(e);
        }
    }
    listEye.clear();
}

void BackgroundMenu::reload(Registry& registry, int eyeCount, const std::vector<Entity>& existingEntities)
{
    destroy(registry);

    listEyeId.push_back(std::make_pair<std::string, std::string>("eyeOutline", "eyePupil"));
    listEyeId.push_back(std::make_pair<std::string, std::string>("eyeOutline2", "eyePupil2"));
    listEyeId.push_back(std::make_pair<std::string, std::string>("eyeOutline3", "eyePupil3"));

    // create background sprite entity (uses the same background used elsewhere)
    m_background = factories::createSprite(registry, "MenuBackground", 0.0f, 0.0f,
        static_cast<float>(SCREEN_WIDTH),
        static_cast<float>(SCREEN_HEIGHT),
        SCREEN_WIDTH, SCREEN_HEIGHT);

    // Random generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // size range for outlines (rendered size)
    const int MIN_SIZE = 32;
    const int MAX_SIZE = 260;

    // we'll try to place without overlap, keep placed rects
    struct Rect {
        float x, y, w, h;
    };
    std::vector<Rect> placed;
    placed.reserve(eyeCount);

    std::uniform_int_distribution<int> sizeDist(MIN_SIZE, MAX_SIZE);
    // positions (centered) within screen with margin
    const float MARGIN = 16.0f;

    std::uniform_real_distribution<float> xDist(MARGIN, SCREEN_WIDTH - MARGIN);
    std::uniform_real_distribution<float> yDist(MARGIN, SCREEN_HEIGHT - MARGIN);
    std::uniform_int_distribution<int> rdmChoice(0, static_cast<int>(listEyeId.size() - 1));

    const int MAX_ATTEMPTS_PER_EYE = 120;

    for (int i = 0; i < eyeCount; ++i) {
        bool placedOk = false;
        for (int attempt = 0; attempt < MAX_ATTEMPTS_PER_EYE && !placedOk; ++attempt) {
            int w = sizeDist(gen);
            int h = w;

            float cx = xDist(gen);
            float cy = yDist(gen);

            int choice = rdmChoice(gen);

            // convert to top-left
            float x = cx - w * 0.5f;
            float y = cy - h * 0.5f;

            // clamp inside screen
            if (x < 0.0f)
                x = 0.0f;
            if (y < 0.0f)
                y = 0.0f;
            if (x + w > SCREEN_WIDTH)
                x = SCREEN_WIDTH - w;
            if (y + h > SCREEN_HEIGHT)
                y = SCREEN_HEIGHT - h;

            // check overlap with existing eyes
            bool overlap = false;
            for (auto& r : placed) {
                if (rectsOverlap(x, y, static_cast<float>(w), static_cast<float>(h),
                        r.x, r.y, r.w, r.h, std::max(4.0f, std::min(w, h) * 0.09f))) {
                    overlap = true;
                    break;
                }
            }
            if (overlap)
                continue;
            overlap = false;
            for (auto& entity : existingEntities) {
                if (registry.has<Sprite>(entity)) {
                    Sprite spr = registry.get<Sprite>(entity);
                    auto& r = spr.dstRect;
                    if (rectsOverlap(x, y, static_cast<float>(w), static_cast<float>(h),
                            r.x + r.w / 3, r.y + r.h / 3, r.w - r.w / 3, r.h - r.h / 3, std::max(4.0f, std::min(w, h) * 0.09f))) {
                        overlap = true;
                        break;
                    }
                }
            }
            if (overlap)
                continue;

            // choose pupil size as a fraction of outline
            float pupilRadius = std::max(3.0f, std::min(w, h) * 0.07f);

            // create the eye: wS/hS (sprite sheet size) we keep equal to outline size for simplicity
            Entity eye = factories::createEye(registry,
                listEyeId[choice].first, listEyeId[choice].second,
                x, y,
                w, h,
                500, 500, // wS/hS sheet/frame sizes - adapt if you have real sprite sheets
                w, h,
                pupilRadius);
            if (eye.id == 0) {
                // failed to create; skip
                break;
            }

            // ensure the pupil/dstRect are properly sized and centered by the createEye factory.
            listEye.push_back(eye);
            placed.push_back({ x, y, static_cast<float>(w), static_cast<float>(h) });
            placedOk = true;
        }

        if (!placedOk) {
            // couldn't place this eye without overlap after many attempts -> skip
            std::cout << "BackgroundMenu: skipped placing eye #" << i << " due to spacing constraints\n";
        }
    }
}

void BackgroundMenu::draw(GraphicsManager& gfx, Registry& registry)
{
    // draw background first (if valid)
    if (m_background.id != 0 && registry.has<Sprite>(m_background)) {
        drawSprite(gfx, registry, m_background);
    }

    // draw eyes: outline behind pupil, drawEye handles order
    for (Entity e : listEye) {
        if (e.id == 0)
            continue;
        if (!registry.has<Eye>(e))
            continue;
        drawEye(gfx, registry, e);
    }
}
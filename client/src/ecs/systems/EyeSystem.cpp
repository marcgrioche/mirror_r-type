/*
** EyeSystem.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/systems
*/

#include "EyeSystem.hpp"
#include "../../managers/GraphicsManager.hpp"
#include "components/Eye.hpp"
#include "components/Position.hpp"
#include "components/Sprite.hpp"
#include <SDL2/SDL.h>
#include <cmath>

void eyeSystem(Registry& registry, float /*deltaTime*/)
{
    int windowX = 0, windowY = 0;
    SDL_GetMouseState(&windowX, &windowY);

    // Convert window coordinates to logical coordinates
    int mx = 0, my = 0;
    GraphicsManager::getInstance().windowToLogical(windowX, windowY, mx, my);

    auto view = registry.view<Eye, Position>();
    for (auto it = view.begin(); it != view.end(); ++it) {
        Entity e = it.entity();

        if (!registry.has<Eye>(e) || !registry.has<Position>(e))
            continue;

        Eye& eye = registry.get<Eye>(e);
        Position& pos = registry.get<Position>(e);

        // center of the eye/pupil in world coords (assume pos is top-left of the sprites)
        float cx = pos.v.x + (eye.pupil_w * 0.5f);
        float cy = pos.v.y + (eye.pupil_h * 0.5f);

        // vector from eye center to mouse
        float dx = static_cast<float>(mx) - cx;
        float dy = static_cast<float>(my) - cy;
        float dist = std::sqrt(dx * dx + dy * dy);

        float maxr = eye.pupil_radius;
        float nx = 0.0f, ny = 0.0f;
        if (dist > 1e-6f) {
            float clamp = (dist > maxr) ? (maxr / dist) : 1.0f;
            nx = dx * clamp;
            ny = dy * clamp;
        }

        // target center of the pupil (clamped)
        float pupilCenterX = cx + nx;
        float pupilCenterY = cy + ny;

        // convert center -> top-left for storing Position / dstRect
        float px = pupilCenterX - (eye.pupil_w * 0.5f);
        float py = pupilCenterY - (eye.pupil_h * 0.5f);

        if (eye.pupilEntity.id == 0)
            continue;

        // update Position component (top-left)
        if (registry.has<Position>(eye.pupilEntity)) {
            Position& ppos = registry.get<Position>(eye.pupilEntity);
            ppos.v.x = px;
            ppos.v.y = py;
        } else {
            registry.emplace<Position>(eye.pupilEntity, px, py);
        }

        // update sprite dstRect to match new top-left
        if (registry.has<Sprite>(eye.pupilEntity)) {
            Sprite& pspr = registry.get<Sprite>(eye.pupilEntity);
            pspr.dstRect.x = static_cast<int>(std::round(px));
            pspr.dstRect.y = static_cast<int>(std::round(py));
            pspr.dstRect.w = eye.pupil_w;
            pspr.dstRect.h = eye.pupil_h;
        }
    }
}
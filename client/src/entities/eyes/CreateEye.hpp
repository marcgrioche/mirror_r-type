/*
** CreateEye.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/entities/eyes
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Oct 20 8:25:53 PM 2025 jojo
** Last update Wed Oct 21 12:31:23 AM 2025 jojo
*/

#pragma once
#include "Registry.hpp"
#include "components/Eye.hpp"
#include "components/Hitbox.hpp"
#include "components/Position.hpp"
#include "components/Sprite.hpp"
#include "components/SpriteFactory.hpp"

/**
 * Create an eye entity composed of:
 *  - parent entity with Position + Eye component
 *  - outline sprite entity
 *  - pupil sprite entity
 *
 * textureOutlineId / texturePupilId can be a path or an id loaded in ResourceManager.
 *
 * width/height = desired rendered size of the eye (outline)
 * pupilW/pupilH = desired rendered size of the pupil sprite
 * pupilRadius = maximum distance pupil center can move from eye center
 */
namespace factories {
Entity createEye(Registry& registry,
    const std::string& textureOutlineId,
    const std::string& texturePupilId,
    float x, float y,
    int width, int height,
    int wS, int hS,
    int pupilW = 16, int pupilH = 16,
    float pupilRadius = 8.0f);
}
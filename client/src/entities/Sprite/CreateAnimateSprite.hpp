/*
** createAnimateSprite.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/entities/Sprite
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Oct 20 5:07:54 PM 2025 jojo
** Last update Tue Oct 20 5:48:21 PM 2025 jojo
*/

#pragma once
#include "Registry.hpp"
#include "components/Hitbox.hpp"
#include "components/Position.hpp"
#include "components/Sprite.hpp"
#include "components/SpriteFactory.hpp"

namespace factories {

/**
 * @brief Creates an animated sprite entity and attaches Position and Sprite components.
 *        The Sprite is configured with the provided texture id, destination size (width/height),
 *        source frame size (wRect/hRect), optional offsets and animation parameters (nbrFrame, framDuration).
 * @param registry The ECS registry to create the entity in
 * @param textureId Texture identifier or path to use for the sprite
 * @param x X position for the sprite (world coordinates)
 * @param y Y position for the sprite (world coordinates)
 * @param width Desired rendered width of the sprite
 * @param height Desired rendered height of the sprite
 * @param wRect Source frame width in the texture (if 0, width is used)
 * @param hRect Source frame height in the texture (if 0, height is used)
 * @param offsetX Optional X offset applied to the sprite rendering
 * @param offsetY Optional Y offset applied to the sprite rendering
 * @param nbrFrame Number of frames in the animation
 * @param framDuration Duration of each frame in seconds
 * @return The created entity
 */
Entity createSprite(Registry& registry, const std::string& textureId, float x, float y,
    float width, float height, float wRect = 1, float hRect = 1, float offsetX = 0, float offsetY = 0,
    int nbrFrame = 1, float framDuration = 0.1f);
}
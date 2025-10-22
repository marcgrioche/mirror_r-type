/*
** Sprite.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/components
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Oct 6 10:44:53 AM 2025 jojo
** Last update Tue Oct 6 10:44:54 AM 2025 jojo
*/

#include "Vector2.hpp"
#include <SDL.h>
#include <string>

#pragma once

/**
 * @brief Component representing a visual sprite for rendering entities (SIMPLIFIED VERSION)
 * @param texture_id ID referencing a texture in the resource manager
 * @param size Width and height of the sprite in pixels (frame dimensions for animated)
 * @param scale Independent X/Y scaling factors for precise hitbox matching
 * @param offset X/Y offset for sprite positioning relative to entity position
 * @param rotation Rotation angle in degrees
 * @param total_frames Total number of animation frames (1 for static sprites)
 * @param current_frame Current animation frame index
 * @param frame_duration Time in seconds each animation frame is displayed
 * @param elapsed_time Time elapsed since last frame change
 */
struct Sprite {
    std::string texture_id; // ID referencing a texture in a texture manager

    // Dimensions & Transform
    Vector2 size;       // Width/height of sprite (or frame if animated)
    Vector2 scale;      // Independent X/Y scaling
    Vector2 offset;     // X/Y offset for positioning
    float rotation = 0.0f; // Rotation in degrees

    // Animation
    int total_frames = 1;
    int current_frame = 0;
    float frame_duration = 0.1f;
    float elapsed_time = 0.0f;
};

/*
** Sprite.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/components
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Oct 6 10:44:53 AM 2025 jojo
** Last update Thu Oct 29 6:09:13 PM 2025 jojo
*/

#include "components/Color.hpp"
#include <SDL.h>
#include <string>

#pragma once

/**
 * @brief Component representing a visual sprite for rendering entities
 * @param texture_id ID referencing a texture in the resource manager
 * @param srcRect Source rectangle defining the area to render from the texture/sprite-sheet
 * @param dstRect Destination rectangle defining position and size on screen
 * @param offset_x X offset for sprite positioning relative to entity position
 * @param offset_y Y offset for sprite positioning relative to entity position
 * @param scale Legacy single scale factor (maintains aspect ratio)
 * @param scale_x Independent horizontal scaling factor for precise hitbox matching
 * @param scale_y Independent vertical scaling factor for precise hitbox matching
 * @param rotation Rotation angle in degrees
 * @param total_frames Total number of animation frames (1 for static sprites)
 * @param current_frame Current animation frame index
 * @param frame_duration Time in seconds each animation frame is displayed
 * @param elapsed_time Time elapsed since last frame change
 * @param frame_width Width of each animation frame in pixels
 * @param frame_height Height of each animation frame in pixels
 */
struct Sprite {
    std::string texture_id; // ID referencing a texture in a texture manager, not raw SDL_Texture, too big
    std::string attack_id;
    std::string current_id;
    SDL_Rect srcRect; // texture rect in sprite-sheet
    SDL_Rect dstRect; // position/size on the screen
    int offset_x = 0;
    int offset_y = 0;
    float scale = 1.0f; // Legacy single scale (maintains aspect ratio)
    float scale_x = 1.0f; // Independent X scaling for exact hitbox matching
    float scale_y = 1.0f; // Independent Y scaling for exact hitbox matching
    float rotation = 0.0f; // in degrees
    int total_frames = 1; // for animated sprites
    int current_frameX = 0;
    int current_frameY = 0;
    float frame_duration = 0.1f;
    float elapsed_time = 0.0f;
    int frame_width = 0;
    int frame_height = 0;
    int nb_state = 0;
    Color color = { 255, 255, 255, 255 };
};

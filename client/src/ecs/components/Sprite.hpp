/*
** Sprite.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/components
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Oct 6 10:44:53 AM 2025 jojo
** Last update Tue Oct 6 10:44:54 AM 2025 jojo
*/

#include <SDL.h>
#include <string>

#pragma once

struct Sprite {
    std::string texture_id; // ID referencing a texture in a texture manager, not raw SDL_Texture, too big
    SDL_Rect srcRect; // texture rect in sprite-sheet
    SDL_Rect dstRect; // position/size on the screen
    int offset_x = 0;
    int offset_y = 0;
    float scale = 1.0f; // Legacy single scale (maintains aspect ratio)
    float scale_x = 1.0f; // Independent X scaling for exact hitbox matching
    float scale_y = 1.0f; // Independent Y scaling for exact hitbox matching
    float rotation = 0.0f; // in degrees
    int total_frames = 1; // for animated sprites
    int current_frame = 0;
    float frame_duration = 0.1f;
    float elapsed_time = 0.0f;
    int frame_width = 0;
    int frame_height = 0;
};

/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Sprite Component
*/

#pragma once

struct Sprite {
    int texture_id; // ID referencing a texture in a texture manager, not raw SDL_Texture, too big
    int width;
    int height;
    int offset_x = 0;
    int offset_y = 0;
    float scale = 1.0f;
    float rotation = 0.0f; // in degrees
    int total_frames = 1; // for animated sprites
    int current_frame = 0;
    float frame_duration = 0.1f;
    float elapsed_time = 0.0f;
    int frame_width = 0;
    int frame_height = 0;
};
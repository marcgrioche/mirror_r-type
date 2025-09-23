/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Components
*/

#pragma once

struct Position {
    float x, y;
};

struct Velocity {
    float dx, dy;
};

struct Health {
    int hp;
};

struct Weight {
    float value;
};

struct InputControlled {
};

struct Sprite {
    std::string texture_id; // ID referencing a texture in a texture manager, not raw SDL_Texture, too big
    SDL_Rect srcRect;
    SDL_Rect dstRect;
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

struct Hitbox {
    float width = 1.0f;
    float height = 1.0f;
    float offset_x = 0.0f;
    float offset_y = 0.0f;
    // bool is_trigger = false;
    // uint32_t layer = 1u;      // bitmask layer
    // uint32_t mask = 0xFFFFFFFFu; // which layers this hitbox collides with
    // bool one_way = false;
};

struct Projectile { };

struct Damage {
    float value = 0.0f;
};

struct Lifetime {
    float value = 0.0f;
};

struct OwnerId {
    int id;
};

struct Vector2 {
    float x = 0.0f;
    float y = 0.0f;
};

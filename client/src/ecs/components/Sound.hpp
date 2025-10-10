/*
** Sound.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/components
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Oct 6 10:44:53 AM 2025 jojo
** Last update Tue Oct 6 10:44:54 AM 2025 jojo
*/

#pragma once

#include <string>

struct Sound {
    std::string sound_id; // Reference to sound in SoundManager
    bool play_on_spawn = true; // Play when entity is created
    bool loop = false; // Loop the sound
    int volume = 128; // 0-128 SDL_mixer volume
    bool is_playing = false; // Current playback state
    int channel = -1; // SDL_mixer channel (-1 = auto-assign)
    bool unique_playback = false; // Prevent overlapping for same sound_id
};

/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SoundSystem
*/

#include "SoundSystem.hpp"
#include "components/Sound.hpp"
#include "managers/SoundManager.hpp"

void soundSystem(Registry& registry, float deltaTime)
{

    (void)deltaTime;
    auto& soundManager = SoundManager::getInstance();

    auto view = registry.view<Sound>();

    for (auto it = view.begin(); it != view.end(); ++it) {
        Entity e = it.entity();
        Sound& sound = registry.get<Sound>(e);

        if (sound.play_on_spawn && !sound.is_playing) {
            if (sound.unique_playback && soundManager.isSoundPlaying(sound.sound_id)) {
                continue;
            }

            int loops = sound.loop ? -1 : 0; // -1 for infinite loop, 0 for play once
            sound.channel = soundManager.playSound(sound.sound_id, loops, sound.volume);
            if (sound.channel >= 0) {
                sound.is_playing = true;
            }
        }

        if (sound.is_playing && !sound.loop && sound.channel >= 0) {
            if (!Mix_Playing(sound.channel)) {
                sound.is_playing = false;
                sound.channel = -1;
            }
        }
    }
}

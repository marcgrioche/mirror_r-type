/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SoundManager
*/

#pragma once

#include <SDL_mixer.h>
#include <string>
#include <unordered_map>

class SoundManager {
public:
    static SoundManager& getInstance();

    bool loadSound(const std::string& id, const std::string& path);
    Mix_Chunk* getSound(const std::string& id) const;
    int playSound(const std::string& id, int loops = 0, int volume = 128);
    void stopChannel(int channel);
    void stopSound(const std::string& id);
    bool isSoundPlaying(const std::string& id) const;
    void setVolume(int channel, int volume);
    void unloadAll();

private:
    SoundManager();
    ~SoundManager();

    bool audio_available;
    std::unordered_map<std::string, Mix_Chunk*> sounds;
    std::unordered_map<std::string, int> playing_sounds;
};

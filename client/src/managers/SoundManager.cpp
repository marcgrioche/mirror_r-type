/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SoundManager
*/

#include "SoundManager.hpp"
#include <iostream>

SoundManager& SoundManager::getInstance()
{
    static SoundManager instance;
    return instance;
}

SoundManager::SoundManager()
{
    audio_available = false;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        std::cout << "Sound system will run in silent mode (no audio available)" << std::endl;
    } else {
        audio_available = true;
        std::cout << "Audio system initialized successfully" << std::endl;
    }
}

SoundManager::~SoundManager()
{
    unloadAll();
    Mix_CloseAudio();
}

bool SoundManager::loadSound(const std::string& id, const std::string& path)
{
    if (!audio_available) {
        std::cout << "Audio not available - skipping sound load: " << id << std::endl;
        return false;
    }

    std::cout << "Loading sound: " << id << " from path: " << path << std::endl;
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (chunk) {
        sounds[id] = chunk;
        std::cout << "Successfully loaded sound: " << id << std::endl;
        return true;
    }
    std::cerr << "Failed to load sound: " << path << " - " << Mix_GetError() << std::endl;
    return false;
}

Mix_Chunk* SoundManager::getSound(const std::string& id) const
{
    auto it = sounds.find(id);
    return (it != sounds.end()) ? it->second : nullptr;
}

int SoundManager::playSound(const std::string& id, int loops, int volume)
{
    if (!audio_available) {
        return -1;
    }

    Mix_Chunk* chunk = getSound(id);
    if (!chunk) {
        std::cerr << "Sound not found: " << id << std::endl;
        return -1;
    }

    Mix_VolumeChunk(chunk, volume);

    int channel = Mix_PlayChannel(-1, chunk, loops);
    if (channel >= 0) {
        playing_sounds[id] = channel;
    } else {
        std::cerr << "Failed to play sound: " << Mix_GetError() << std::endl;
    }
    return channel;
}

void SoundManager::stopChannel(int channel)
{
    if (!audio_available || channel < 0) {
        return;
    }
    Mix_HaltChannel(channel);
}

void SoundManager::setVolume(int channel, int volume)
{
    if (!audio_available || channel < 0) {
        return;
    }
    Mix_Volume(channel, volume);
}

void SoundManager::stopSound(const std::string& id)
{
    if (!audio_available) {
        return;
    }

    auto it = playing_sounds.find(id);
    if (it != playing_sounds.end()) {
        Mix_HaltChannel(it->second);
        playing_sounds.erase(it);
    }
}

bool SoundManager::isSoundPlaying(const std::string& id) const
{
    if (!audio_available) {
        return false;
    }

    auto it = playing_sounds.find(id);
    if (it != playing_sounds.end()) {
        return Mix_Playing(it->second) != 0;
    }
    return false;
}

void SoundManager::unloadAll()
{
    for (auto& kv : sounds) {
        Mix_FreeChunk(kv.second);
    }
    sounds.clear();
}

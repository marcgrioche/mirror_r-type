/*
** SoundManager.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/managers
**
** Made by jojo
** Login   <jojo>
**
** Started on  Thu Oct 30 1:36:00 PM 2025 jojo
** Last update Fri Oct 30 3:14:44 PM 2025 jojo
*/

#include "SoundManager.hpp"
#include "ResourceManager.hpp"
#include <iostream>

SoundManager& SoundManager::getInstance()
{
    static SoundManager instance;
    return instance;
}

bool SoundManager::initialize(int frequency, Uint16 format, int channels, int chunksize)
{
    if (m_initialized)
        return true;

    int want = MIX_INIT_OGG | MIX_INIT_MP3;
    int initted = Mix_Init(want);
    if ((initted & want) != want) {
        std::cerr << "Warning: Mix_Init missing codecs: " << Mix_GetError() << std::endl;
    }

    if (Mix_OpenAudio(frequency, format, channels, chunksize) != 0) {
        std::cerr << "Error: Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
        Mix_Quit();
        return false;
    }

    // Autorise la lecture simultanée de plusieurs SFX
    Mix_AllocateChannels(32);
    Mix_Volume(-1, m_chunkVolume);
    Mix_VolumeMusic(m_musicVolume);

    char* basePath = SDL_GetBasePath();
    if (basePath) {
        basePath_ = basePath;
        SDL_free(basePath);
    } else {
        std::cerr << "Warning: Could not get base path, using relative paths" << std::endl;
        basePath_ = "";
        // defaultFontPath = "res/fonts/OpenSans-Medium.ttf";
    }

    m_initialized = true;
    return true;
}

std::string ResourceManager::getAssetPath(const std::string& relativePath) const
{
    return basePath_ + "res/" + relativePath;
}

void SoundManager::cleanup()
{
    unloadAll();
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
    if (m_initialized) {
        Mix_CloseAudio();
        Mix_Quit();
    }
    m_initialized = false;
}

Mix_Chunk* SoundManager::loadChunk(const std::string& id, const std::string& path)
{
    auto it = m_chunks.find(id);
    if (it != m_chunks.end())
        return it->second;

    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (!chunk) {
        std::cerr << "Error: Mix_LoadWAV failed for '" << path << "': " << Mix_GetError() << std::endl;
        return nullptr;
    }
    m_chunks[id] = chunk;
    Mix_VolumeChunk(chunk, m_chunkVolume);
    return chunk;
}

Mix_Music* SoundManager::loadMusic(const std::string& id, const std::string& path)
{
    auto it = m_music.find(id);
    if (it != m_music.end())
        return it->second;

    Mix_Music* mus = Mix_LoadMUS(path.c_str());
    if (!mus) {
        std::cerr << "Error: Mix_LoadMUS failed for '" << path << "': " << Mix_GetError() << std::endl;
        return nullptr;
    }
    m_music[id] = mus;
    Mix_VolumeMusic(m_musicVolume);
    return mus;
}

Mix_Chunk* SoundManager::loadChunkFromAsset(const std::string& id, const std::string& relativeAssetPath)
{
    auto& rm = ResourceManager::getInstance();
    return loadChunk(id, rm.getAssetPath(relativeAssetPath));
}

Mix_Music* SoundManager::loadMusicFromAsset(const std::string& id, const std::string& relativeAssetPath)
{
    auto& rm = ResourceManager::getInstance();
    return loadMusic(id, rm.getAssetPath(relativeAssetPath));
}

Mix_Chunk* SoundManager::getChunk(const std::string& id) const
{
    auto it = m_chunks.find(id);
    return (it != m_chunks.end()) ? it->second : nullptr;
}

Mix_Music* SoundManager::getMusic(const std::string& id) const
{
    auto it = m_music.find(id);
    return (it != m_music.end()) ? it->second : nullptr;
}

int SoundManager::playChunk(const std::string& id, int loops, int channel, int volume)
{
    Mix_Chunk* chunk = getChunk(id);
    if (!chunk) {
        std::cerr << "Warning: playChunk: chunk '" << id << "' not found" << std::endl;
        return -1;
    }
    if (volume >= 0) {
        Mix_VolumeChunk(chunk, clampVolume(volume));
    }
    int ch = Mix_PlayChannel(channel, chunk, loops);
    if (ch == -1) {
        std::cerr << "Warning: Mix_PlayChannel failed: " << Mix_GetError() << std::endl;
    }
    return ch;
}

bool SoundManager::playMusic(const std::string& id, int loops, int fadeMs, int volume)
{
    Mix_Music* mus = getMusic(id);
    if (!mus) {
        std::cerr << "Warning: playMusic: music '" << id << "' not found" << std::endl;
        return false;
    }
    if (volume >= 0) {
        Mix_VolumeMusic(clampVolume(volume));
    }
    int rc = (fadeMs > 0) ? Mix_FadeInMusic(mus, loops, fadeMs) : Mix_PlayMusic(mus, loops);
    if (rc != 0) {
        std::cerr << "Warning: Mix_PlayMusic failed: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void SoundManager::stopMusic(int fadeOutMs)
{
    if (fadeOutMs > 0) {
        if (Mix_FadeOutMusic(fadeOutMs) == 0) {
            Mix_HaltMusic();
        }
    } else {
        Mix_HaltMusic();
    }
}

void SoundManager::setChunkVolume(int volume)
{
    m_chunkVolume = clampVolume(volume);
    Mix_Volume(-1, m_chunkVolume);
    for (auto& kv : m_chunks) {
        Mix_VolumeChunk(kv.second, m_chunkVolume);
    }
}

void SoundManager::setMusicVolume(int volume)
{
    m_musicVolume = clampVolume(volume);
    Mix_VolumeMusic(m_musicVolume);
}

void SoundManager::unloadAll()
{
    for (auto& kv : m_chunks) {
        Mix_FreeChunk(kv.second);
    }
    m_chunks.clear();

    for (auto& kv : m_music) {
        Mix_FreeMusic(kv.second);
    }
    m_music.clear();
}

SoundManager::~SoundManager()
{
    cleanup();
}

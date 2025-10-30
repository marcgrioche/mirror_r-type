/*
** SoundManager.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/managers
**
** Made by jojo
** Login   <jojo>
**
** Started on  Thu Oct 30 1:35:49 PM 2025 jojo
** Last update Fri Oct 30 3:13:45 PM 2025 jojo
*/

#pragma once
#include <SDL_mixer.h>
#include <string>
#include <unordered_map>

class SoundManager {
public:
    static SoundManager& getInstance();

    bool initialize(int frequency = 44100, Uint16 format = MIX_DEFAULT_FORMAT, int channels = 2, int chunksize = 1024);
    void cleanup();
    bool isInitialized() const { return m_initialized; }

    Mix_Chunk* loadChunk(const std::string& id, const std::string& path);
    Mix_Music* loadMusic(const std::string& id, const std::string& path);

    Mix_Chunk* loadChunkFromAsset(const std::string& id, const std::string& relativeAssetPath);
    Mix_Music* loadMusicFromAsset(const std::string& id, const std::string& relativeAssetPath);

    Mix_Chunk* getChunk(const std::string& id) const;
    Mix_Music* getMusic(const std::string& id) const;

    // -1 channel => premier canal libre. loops: 0=une fois, -1=infini.
    int playChunk(const std::string& id, int loops = 0, int channel = -1, int volume = -1);
    bool playMusic(const std::string& id, int loops = -1, int fadeMs = 0, int volume = -1);
    void stopMusic(int fadeOutMs = 0);

    void setChunkVolume(int volume); // 0..128
    void setMusicVolume(int volume); // 0..128
    int getChunkVolume() const { return m_chunkVolume; }
    int getMusicVolume() const { return m_musicVolume; }

    void unloadAll();

    std::string getAssetPath(const std::string& relativePath) const;

private:
    SoundManager() = default;
    ~SoundManager();

    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;

    static int clampVolume(int v) { return (v < 0) ? 0 : (v > MIX_MAX_VOLUME ? MIX_MAX_VOLUME : v); }

    bool m_initialized = false;
    int m_chunkVolume = MIX_MAX_VOLUME;
    int m_musicVolume = MIX_MAX_VOLUME;

    std::unordered_map<std::string, Mix_Chunk*> m_chunks;
    std::unordered_map<std::string, Mix_Music*> m_music;

    std::string basePath_;
};

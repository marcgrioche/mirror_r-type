#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <unordered_map>

class ResourceManager {
    public:
        static ResourceManager& getInstance();

        SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& id, const std::string& path);
        SDL_Texture* getTexture(const std::string& id) const;
        void unloadAll();

    private:
        ResourceManager() = default;
        ~ResourceManager();

        std::unordered_map<std::string, SDL_Texture*> textures;
};


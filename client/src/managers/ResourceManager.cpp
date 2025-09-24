#include "ResourceManager.hpp"
#include <iostream>

ResourceManager& ResourceManager::getInstance()
{
    static ResourceManager instance;
    return instance;
}

SDL_Texture* ResourceManager::loadTexture(SDL_Renderer* renderer, const std::string& id, const std::string& path)
{
    SDL_Texture* tex = IMG_LoadTexture(renderer, path.c_str());
    if (tex)
        textures[id] = tex;
    else
        std::cout << "Texture not loaded due to an unexpected problem" << std::endl;
    return tex;
}

SDL_Texture* ResourceManager::getTexture(const std::string& id) const
{
    auto it = textures.find(id);
    return (it != textures.end()) ? it->second : nullptr;
}

void ResourceManager::unloadAll()
{
    for (auto& kv : textures) {
        SDL_DestroyTexture(kv.second);
    }
    textures.clear();
}

ResourceManager::~ResourceManager()
{
    unloadAll();
}

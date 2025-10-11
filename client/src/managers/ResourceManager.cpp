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
    if (tex) {
        textures[id] = tex;
    }
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
    for (auto it = fonts.begin(); it != fonts.end(); ++it) {
        if (it->second) {
            TTF_CloseFont(it->second);
        }
    }
    fonts.clear();
}

void ResourceManager::loadFonts(const std::string& t_fontPath, const int t_fontSize)
{
    auto loaded = fonts.find(t_fontSize);
    if (loaded == fonts.end()) {
        fonts[t_fontSize] = TTF_OpenFont(t_fontPath.c_str(), t_fontSize);
        if (!fonts[t_fontSize]) {
            std::cerr << "Failed to load default font: " << TTF_GetError() << std::endl;
        }
    }
}

TTF_Font* ResourceManager::getFont(const int t_fontSize)
{
    if (!fonts[t_fontSize]) {
        fonts[t_fontSize] = TTF_OpenFont(defaultFontPath.c_str(), t_fontSize);
        if (!fonts[t_fontSize]) {
            std::cerr << "Failed to load default font: " << TTF_GetError() << std::endl;
            return nullptr;
        }
    }
    return fonts[t_fontSize];
}

ResourceManager::~ResourceManager()
{
    unloadAll();
}

#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <unordered_map>

class ResourceManager {
public:
    static ResourceManager& getInstance();

    SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& id, const std::string& path);
    SDL_Texture* getTexture(const std::string& id) const;
    void loadFonts(const std::string& t_fontPath, int t_fontSize);
    TTF_Font* getFont(int t_fontSize);
    void unloadAll();

private:
    ResourceManager() = default;
    ~ResourceManager();

    std::unordered_map<std::string, SDL_Texture*> textures;
    std::string defaultFontPath = "client/res/fonts/OpenSans-Medium.ttf";
    std::unordered_map<int, TTF_Font*> fonts;
};

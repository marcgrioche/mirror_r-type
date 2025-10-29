#include "ResourceManager.hpp"
#include <iostream>

ResourceManager& ResourceManager::getInstance()
{
    static ResourceManager instance;
    return instance;
}

void ResourceManager::initialize()
{
    char* basePath = SDL_GetBasePath();
    if (basePath) {
        basePath_ = basePath;
        SDL_free(basePath);
        defaultFontPath = basePath_ + "res/fonts/OpenSans-Medium.ttf";
    } else {
        std::cerr << "Warning: Could not get base path, using relative paths" << std::endl;
        basePath_ = "";
        defaultFontPath = "res/fonts/OpenSans-Medium.ttf";
    }
}

std::string ResourceManager::getAssetPath(const std::string& relativePath) const
{
    return basePath_ + "res/" + relativePath;
}

std::string ResourceManager::getBasePath() const
{
    return basePath_;
}

SDL_Texture* ResourceManager::loadTexture(SDL_Renderer* renderer, const std::string& id, const std::string& path)
{
    // Try the straightforward load first
    SDL_Texture* tex = IMG_LoadTexture(renderer, path.c_str());
    if (tex) {
        textures[id] = tex;
        textureScales[id] = 1.0f;
        return tex;
    }

    // Fallback: try loading as surface and downscale if too large for the renderer
    SDL_Surface* surf = IMG_Load(path.c_str());
    if (!surf) {
        return nullptr;
    }

    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(renderer, &info) != 0) {
        // If we can't query limits, assume conservative defaults
        info.max_texture_width = 8192;
        info.max_texture_height = 8192;
    }

    int maxW = (info.max_texture_width > 0) ? static_cast<int>(info.max_texture_width) : 8192;
    int maxH = (info.max_texture_height > 0) ? static_cast<int>(info.max_texture_height) : 8192;

    float scale = 1.0f;
    if (surf->w > maxW || surf->h > maxH) {
        float scaleW = static_cast<float>(maxW) / static_cast<float>(surf->w);
        float scaleH = static_cast<float>(maxH) / static_cast<float>(surf->h);
        scale = (scaleW < scaleH) ? scaleW : scaleH;
    }

    SDL_Texture* resultTex = nullptr;
    if (scale < 1.0f) {
        int newW = static_cast<int>(surf->w * scale);
        int newH = static_cast<int>(surf->h * scale);
        if (newW <= 0) newW = 1;
        if (newH <= 0) newH = 1;

        SDL_Surface* scaled = SDL_CreateRGBSurfaceWithFormat(0, newW, newH, surf->format->BitsPerPixel, surf->format->format);
        if (scaled) {
            SDL_Rect src { 0, 0, surf->w, surf->h };
            SDL_Rect dst { 0, 0, newW, newH };
            SDL_BlitScaled(surf, &src, scaled, &dst);
            resultTex = SDL_CreateTextureFromSurface(renderer, scaled);
            SDL_FreeSurface(scaled);
        }
    } else {
        // No scaling required, try creating texture from surface
        resultTex = SDL_CreateTextureFromSurface(renderer, surf);
    }

    SDL_FreeSurface(surf);

    if (resultTex) {
        textures[id] = resultTex;
        textureScales[id] = scale;
    }
    return resultTex;
}

SDL_Texture* ResourceManager::getTexture(const std::string& id) const
{
    auto it = textures.find(id);
    return (it != textures.end()) ? it->second : nullptr;
}

float ResourceManager::getTextureScale(const std::string& id) const
{
    auto it = textureScales.find(id);
    if (it != textureScales.end()) {
        return it->second;
    }
    return 1.0f;
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

#include "Position.hpp"
#include "TextBox.hpp"
#include "TextBoxDimensions.hpp"
#include "managers/ResourceManager.hpp"
#include <SDL_ttf.h>

SDL_Rect getTextBoxDimensions(Registry& registry, const Entity& entity)
{
    SDL_Rect rect = { 0, 0, 0, 0 };
    auto& resourceManager = ResourceManager::getInstance();

    if (!registry.has<TextBox>(entity) || !registry.has<Position>(entity)) {
        return rect;
    }

    const TextBox& textbox = registry.get<TextBox>(entity);
    const Position& pos = registry.get<Position>(entity);

    TTF_Font* font = resourceManager.getFont(textbox.fontSize);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        // Essaie un chemin de police par défaut
        return rect;
    }

    int textWidth = 0;
    int textHeight = 0;
    if (TTF_SizeText(font, textbox.text.c_str(), &textWidth, &textHeight) != 0) {
        std::cerr << "Failed to get text size: " << TTF_GetError() << std::endl;
        return rect;
    }

    rect.x = static_cast<int>(pos.v.x);
    rect.y = static_cast<int>(pos.v.y);
    rect.w = textWidth;
    rect.h = textHeight;

    return rect;
}

float getXOffset(const SDL_Rect& rect, const float width)
{
    const float textCenterX = static_cast<float>(rect.x) + static_cast<float>(rect.w) * 0.5f;
    return textCenterX - width * 0.5f;
}

float getYOffset(const SDL_Rect& rect, const float height)
{
    const float textCenterY = static_cast<float>(rect.y) + static_cast<float>(rect.h) * 0.5f;
    return textCenterY - height * 0.5f;
}

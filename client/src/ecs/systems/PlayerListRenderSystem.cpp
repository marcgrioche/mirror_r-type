#include "PlayerListRenderSystem.hpp"
#include "components/Health.hpp"
#include "components/Tags.hpp"
#include "components/TextBox.hpp"
#include "managers/GraphicsManager.hpp"
#include "managers/ResourceManager.hpp"

// Optionally: add a Player tag component if you want to filter only player entities
// #include "components/Player.hpp"

void playerListRenderSystem(Registry& registry, SDL_Renderer* renderer)
{
    // Settings for layout
    const int padding = 10;
    const int nameHeight = 24;
    const int barWidth = 100;
    const int barHeight = 16;
    const int barMaxHp = 100; // or change to fetch max hp per player

    auto& resourceManager = ResourceManager::getInstance();

    int y = padding;

    // For all entities with Health and TextBox (name)
    auto view = registry.view<TextBox, Health>();
    for (auto it = view.begin(); it != view.end(); ++it) {
        Entity e = it.entity();
        TextBox& nameBox = registry.get<TextBox>(e);
        Health& health = registry.get<Health>(e);

        if (!registry.has<PlayerTag>(e)) {
            continue;
        }

        // --- Render Name ---
        TTF_Font* font = resourceManager.getFont(nameBox.fontSize);
        SDL_Color color = { 255, 255, 255, 255 }; // White for name
        SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, nameBox.text.c_str(), color);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect nameRect = { padding, y, textSurface->w, textSurface->h };
            SDL_RenderCopy(renderer, textTexture, nullptr, &nameRect);
            SDL_DestroyTexture(textTexture);
            SDL_FreeSurface(textSurface);
        }

        // --- Render HP Bar ---
        const int hp = health.hp;
        const int hpBarW = (hp * barWidth) / barMaxHp;
        SDL_Rect bgRect = { padding, y + nameHeight, barWidth, barHeight };
        SDL_Rect hpRect = { padding, y + nameHeight, hpBarW, barHeight };

        // Draw background (dark gray)
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_RenderFillRect(renderer, &bgRect);

        // Draw HP (green)
        if (hp < 25) {
            SDL_SetRenderDrawColor(renderer, 200, 60, 60, 255); // Red for low HP
        } else if (hp < 70) {
            SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); // Orange for medium HP
        } else {
            SDL_SetRenderDrawColor(renderer, 60, 200, 60, 255); // Green for normal HP
        }
        SDL_RenderFillRect(renderer, &hpRect);

        // Draw border
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &bgRect);

        y += nameHeight + barHeight + padding;
    }
}
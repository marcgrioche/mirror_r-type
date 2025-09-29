#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../entities/button/CreateButton.hpp"
#include "Entity.hpp" // pour stocker l'entité bouton

#include <string>

class GraphicsManager;
class Registry;

class Menu {
public:
    void activate();
    void deactivate();
    // --- ajouté ---
    void deactivate(Registry& registry);
    // --- fin ajouté ---
    void handleEvent(const SDL_Event& e);
    void render(GraphicsManager& gfx, Registry& registry);
    ~Menu() = default;

    bool shouldStart() const { return m_requestStart; }
    void consumeStartSignal() { m_requestStart = false; }
    const std::string& code() const { return m_inputCode; }

private:
    bool m_active = false;
    bool m_requestStart = false;

    std::string m_inputCode;
    bool m_inputFocused = true;

    SDL_Rect m_inputRect { 800 / 2 - 200, 600 / 2 - 80, 400, 50 };
    SDL_Rect m_buttonRect { 800 / 2 - 100, 600 / 2 + 0, 200, 60 };

    // Text rendering
    TTF_Font* m_font = nullptr;
    int m_fontSize = 22;
    SDL_Color m_textColor { 240, 240, 255, 255 };

    // --- ajouté: entité bouton "join server" ---
    Entity m_joinServerButton;
    bool m_joinButtonCreated = false;
    // --- fin ajouté ---

    void ensureFont();
    std::string findFontPath() const;
    void renderInputText(SDL_Renderer* renderer);
    void renderTextCentered(SDL_Renderer* renderer, const SDL_Rect& rect, const std::string& text, SDL_Color color);
};
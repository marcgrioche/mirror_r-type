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
    enum class Page { Connect, Lobby };

    void activate(Page page = Page::Connect);
    void deactivate();
    void deactivate(Registry& registry);

    void handleEvent(const SDL_Event& e);
    void render(GraphicsManager& gfx, Registry& registry);

    // Navigation “hard-coded”
    void onConnected() { m_page = Page::Lobby; }

    // Signaux UI (hard-coded)
    bool popConnectRequest() { bool v = m_requestConnect; m_requestConnect = false; return v; }
    bool popCreateLobbyRequest() { bool v = m_requestCreate; m_requestCreate = false; return v; }
    bool popJoinLobbyRequest() { bool v = m_requestJoin; m_requestJoin = false; return v; }

    ~Menu() = default;

private:
    // Pages
    void renderConnectPage(GraphicsManager& gfx, Registry& registry);
    void renderLobbyPage(GraphicsManager& gfx, Registry& registry);

    // Etat global
    bool m_active = false;
    Page m_page = Page::Connect;

    // Page Connect
    std::string m_inputCode;
    bool m_inputFocused = true;
    SDL_Rect m_inputRect { 800 / 2 - 200, 600 / 2 - 80, 400, 50 };
    SDL_Rect m_connectBtnRect { 800 / 2 - 100, 600 / 2 + 0, 200, 60 };
    Entity m_joinServerButton {};
    bool m_joinButtonCreated = false;
    bool m_requestConnect = false;

    // Page Lobby
    SDL_Rect m_createBtnRect { 800 / 2 - 220, 600 / 2 - 30, 200, 60 };
    SDL_Rect m_joinBtnRect   { 800 / 2 + 20,  600 / 2 - 30, 200, 60 };
    Entity m_createLobbyButton {};
    Entity m_joinLobbyButton {};
    bool m_lobbyButtonsCreated = false;
    bool m_requestCreate = false;
    bool m_requestJoin = false;

    // Text rendering
    TTF_Font* m_font = nullptr;
    int m_fontSize = 22;
    SDL_Color m_textColor { 240, 240, 255, 255 };

    void ensureFont();
    std::string findFontPath() const;
    void renderInputText(SDL_Renderer* renderer);
    void renderTextCentered(SDL_Renderer* renderer, const SDL_Rect& rect, const std::string& text, SDL_Color color);
};
#pragma once
#include "../entities/button/CreateButton.hpp"
#include "Entity.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

class GraphicsManager;
class Registry;

class Menu {
public:
    enum class Page { Connect,
        Lobby,
        Join,
        Start };

    void activate(Page page = Page::Connect);
    void deactivate();
    void deactivate(std::shared_ptr<Registry> registry);

    void handleEvent(const SDL_Event& e);
    void render(GraphicsManager& gfx, std::shared_ptr<Registry> registry);

    std::string m_inputCode;
    void onConnected() { m_page = Page::Lobby; }
    void onJoint()
    {
        m_page = Page::Join;
        m_inputFocused = true;
        m_inputCode.clear();
        m_joinConfirmCreated = false;
    }
    void onCreated() { m_page = Page::Start; }

    bool popConnectRequest()
    {
        bool v = m_requestConnect;
        m_requestConnect = false;
        return v;
    }
    bool popCreateLobbyRequest()
    {
        bool v = m_requestCreate;
        m_requestCreate = false;
        return v;
    }
    bool popJoinLobbyRequest()
    {
        bool v = m_requestJoin;
        m_requestJoin = false;
        return v;
    }
    bool popStartRequest()
    {
        bool v = m_requestStart;
        m_requestStart = false;
        return v;
    }

    ~Menu() = default;

private:
    void renderConnectPage(GraphicsManager& gfx, std::shared_ptr<Registry> registry);
    void renderLobbyPage(GraphicsManager& gfx, std::shared_ptr<Registry> registry);
    void renderJoinPage(GraphicsManager& gfx, std::shared_ptr<Registry> registry);
    void renderStartPage(GraphicsManager& gfx, std::shared_ptr<Registry> registry);

    bool m_active = false;
    Page m_page = Page::Connect;

    bool m_inputFocused = true;
    SDL_Rect m_inputRect { 800 / 2 - 200, 600 / 2 - 80, 400, 50 };
    SDL_Rect m_connectBtnRect { 800 / 2 - 100, 600 / 2 + 0, 200, 60 };
    Entity m_joinServerButton {};
    bool m_joinButtonCreated = false;
    bool m_requestConnect = false;

    SDL_Rect m_createBtnRect { 800 / 2 - 220, 600 / 2 - 30, 200, 60 };
    SDL_Rect m_joinBtnRect { 800 / 2 + 20, 600 / 2 - 30, 200, 60 };
    Entity m_createLobbyButton {};
    Entity m_joinLobbyButton {};
    bool m_lobbyButtonsCreated = false;
    bool m_requestCreate = false;
    bool m_requestJoin = false;
    bool m_requestStart = false;

    Entity m_joinConfirmButton {};
    bool m_joinConfirmCreated = false;

    SDL_Rect m_startBtnRect { 800 / 2 - 100, 600 / 2 - 30, 200, 60 };
    Entity m_startButton {};
    bool m_startButtonCreated = false;

    TTF_Font* m_font = nullptr;
    int m_fontSize = 22;
    SDL_Color m_textColor { 240, 240, 255, 255 };

    void ensureFont();
    std::string findFontPath() const;
    void renderInputText(SDL_Renderer* renderer);
    void renderTextCentered(SDL_Renderer* renderer, const SDL_Rect& rect, const std::string& text, SDL_Color color);
};

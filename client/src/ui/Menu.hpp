#pragma once
#include "../entities/button/CreateButton.hpp"
#include "Entity.hpp"
#include <SDL.h>
#include <SDL_ttf.h>

#include <string>

class GraphicsManager;
class Registry;

class Menu {
public:
    enum class Page { Connect,
        Lobby,
        Join,
        Start,
        Win,
        Lose };

    void activate(Page page = Page::Connect);
    void deactivate();
    void deactivate(Registry& registry);

    void handleEvent(const SDL_Event& e);
    void render(GraphicsManager& gfx, Registry& registry);

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
    void renderConnectPage(GraphicsManager& gfx, Registry& registry);
    void renderLobbyPage(GraphicsManager& gfx, Registry& registry);
    void renderJoinPage(GraphicsManager& gfx, Registry& registry);
    void renderStartPage(GraphicsManager& gfx, Registry& registry);
    void renderWinPage(GraphicsManager& gfx, Registry& registry);
    void renderLosePage(GraphicsManager& gfx, Registry& registry);

    bool m_active = false;
    Page m_currentPage = Page::HOME;

    // Pages modulaires
    HomeMenu m_homePage;
    ConnectionMenu m_connectionPage;
    JoinMenu m_joinPage;
    LobbyMenu m_lobbyPage;
    EndMenuPage m_endPage;
    // ParameterMenu m_parameterPage;

    // Gestion des transitions
    void hideAllPages(Registry& registry);
    void processPageTransitions(Registry& registry);
};

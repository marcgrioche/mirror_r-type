#pragma once
#include "Entity.hpp"
#include "Registry.hpp"
#include "managers/GraphicsManager.hpp"
#include "page/ConnectionMenu.hpp"
#include "page/EndMenu.hpp"
#include "page/HomeMenu.hpp"
#include "page/JoinMenu.hpp"
#include "page/LobbyMenu.hpp"
#include "page/ParameterMenu.hpp"
#include <SDL.h>

class Menu {
public:
    enum class Page {
        HOME,
        CONNECTION,
        JOIN_LOBBY,
        LOBBY,
        PARAMETERS,
        WIN,
        LOSE
    };

    Menu();
    ~Menu();

    // Interface principale
    void activate(Registry& registry, Page page = Page::HOME);
    void deactivate(Registry& registry);
    bool isActive() const { return m_active; }

    // Gestion des événements et rendu
    void handleEvent(const SDL_Event& e, Registry& registry);
    void update(Registry& registry, float deltaTime);
    void render(GraphicsManager& gfx, Registry& registry);

    // Navigation entre pages
    void showHomePage(Registry& registry);
    void showConnectionPage(Registry& registry);
    void showJoinPage(Registry& registry);
    void showParametersPage(Registry& registry);
    void showWinPage(Registry& registry);
    void showLosePage(Registry& registry);
    void showLobbyPage(Registry& registry);
    void showLobbyPageAfterGame(Registry& registry);

    // Récupération des données saisies
    std::string getConnectionCode(Registry& registry) const;
    std::string getJoinCode(Registry& registry) const;
    std::string getUserPseudo(Registry& registry) const;

    // Vérification des demandes utilisateur
    bool hasConnectionRequest() const;
    bool hasJoinRequest() const;
    bool hasCreateRequest() const;
    bool hasLobbyRequest() const;
    bool hasEndRequest() const;
    bool hasParameterChanges() const;

    // Nettoyage des flags de demande
    void clearAllRequests();

    // Entity management
    void clearGameEntities(Registry& registry);

private:
    bool m_active = false;
    Page m_currentPage = Page::HOME;

    // Pages modulaires
    HomeMenu m_homePage;
    ConnectionMenu m_connectionPage;
    JoinMenu m_joinPage;
    LobbyMenu m_lobbyPage;
    EndMenuPage m_endPage;
    ParameterMenu m_parameterPage;

    // Gestion des transitions
    void hideAllPages(Registry& registry);
    void processPageTransitions(Registry& registry);
};

#include "Menu.hpp"
#include "../entities/button/CreateButton.hpp"
#include "../managers/GraphicsManager.hpp"
#include "components/Button.hpp"
#include "components/Hitbox.hpp"
#include "components/Position.hpp"
#include <filesystem>
#include <iostream>

Menu::Menu()
    = default;
Menu::~Menu() = default;

void Menu::activate(Registry& _registry, Page page)
{
    m_active = true;
    m_currentPage = page;

    switch (page) {
    case Page::HOME:
        m_homePage.show(_registry);
        break;
    case Page::CONNECTION:
        m_connectionPage.show(_registry);
        break;
    case Page::JOIN_LOBBY:
        m_joinPage.show(_registry);
        break;
    case Page::LOBBY:
        m_lobbyPage.show(_registry, 0);
        break;
    case Page::WIN:
        m_endPage.show(_registry, true);
        break;
    case Page::LOSE:
        m_endPage.show(_registry, false);
        break;
    default:
        break;
    }
}

void Menu::deactivate(Registry& registry)
{
    if (!m_active)
        return;

    hideAllPages(registry);
    m_active = false;
}

void Menu::hideAllPages(Registry& registry)
{
    m_homePage.hide(registry);
    m_connectionPage.hide(registry);
    m_joinPage.hide(registry);
    // Hide all other pages to avoid leaving stale UI/entities around
    m_lobbyPage.hide(registry);
    m_endPage.hide(registry);
    m_parameterPage.hide(registry);
}

void Menu::showHomePage(Registry& registry)
{
    hideAllPages(registry);
    m_currentPage = Page::HOME;
    m_homePage.show(registry);
}

void Menu::showConnectionPage(Registry& registry)
{
    hideAllPages(registry);
    m_currentPage = Page::CONNECTION;
    m_connectionPage.show(registry);
}

void Menu::showJoinPage(Registry& registry)
{
    hideAllPages(registry);
    m_currentPage = Page::JOIN_LOBBY;
    m_joinPage.show(registry);
}

void Menu::showLobbyPage(Registry& registry, int lobbyId)
{
    hideAllPages(registry);
    m_currentPage = Page::LOBBY;
    m_lobbyPage.show(registry, lobbyId);
}

void Menu::showLobbyPageAfterGame(Registry& registry)
{
    hideAllPages(registry);
    m_currentPage = Page::LOBBY;
    m_lobbyPage.showAfterGameEnd(registry);
}

void Menu::clearGameEntities(Registry& registry)
{
    (void)registry;
    // This method will be called by the Game class to clear entities
    // Implementation is in Game::clearGameEntities()
}

void Menu::showParametersPage(Registry& registry)
{
    hideAllPages(registry);
    m_currentPage = Page::PARAMETERS;
    m_parameterPage.show(registry);
}

void Menu::showWinPage(Registry& registry)
{
    hideAllPages(registry);
    m_currentPage = Page::WIN;
    // Ensure end page entities/state are (re)created with WIN state
    m_endPage.show(registry, true);
}

void Menu::showLosePage(Registry& registry)
{
    hideAllPages(registry);
    m_currentPage = Page::LOSE;
    // Ensure end page entities/state are (re)created with LOSE state
    m_endPage.show(registry, false);
}

void Menu::handleEvent(const SDL_Event& e, Registry& registry)
{
    if (!m_active)
        return;

    switch (m_currentPage) {
    case Page::HOME:
        m_homePage.handleEvent(registry, e);
        break;
    case Page::CONNECTION:
        m_connectionPage.handleEvent(registry, e);
        break;
    case Page::JOIN_LOBBY:
        m_joinPage.handleEvent(registry, e);
        break;
    case Page::LOBBY:
        m_lobbyPage.handleEvent(registry, e);
        break;
    case Page::PARAMETERS:
        m_parameterPage.handleEvent(registry, e);
        break;
    case Page::WIN:
        m_endPage.handleEvent(registry, e);
        break;
    case Page::LOSE:
        m_endPage.handleEvent(registry, e);
        break;
    default:
        break;
    }
}

void Menu::update(Registry& registry, float deltaTime)
{
    if (!m_active)
        return;

    // Met à jour la page active
    switch (m_currentPage) {
    case Page::HOME:
        m_homePage.update(registry, deltaTime);
        break;
    case Page::CONNECTION:
        m_connectionPage.update(registry, deltaTime);
        break;
    case Page::JOIN_LOBBY:
        m_joinPage.update(registry, deltaTime);
        break;
    case Page::PARAMETERS:
        m_parameterPage.update(registry, deltaTime);
        break;
    default:
        break;
    }

    // Gère les transitions entre pages
    processPageTransitions(registry);
}

void Menu::processPageTransitions(Registry& registry)
{
    switch (m_currentPage) {
    case Page::HOME:
        if (m_homePage.hasJoinRequest()) {
            m_joinPage.setUserName(m_homePage.getPseudo(registry));
            m_homePage.clearRequests();
            showJoinPage(registry);
        } else if (m_homePage.hasCreateRequest()) {
            m_homePage.clearRequests();
            showConnectionPage(registry);
        } else if (m_homePage.hasParamRequest()) {
            m_homePage.clearRequests();
            showParametersPage(registry);
        }
        break;

    case Page::CONNECTION:
        // Transition gérée par le système principal via hasConnectionRequest()
        break;
    case Page::LOBBY:
        if (m_lobbyPage.hasReturnRequest()) {
            m_lobbyPage.clearRequests();
            showHomePage(registry);
        }
        break;
    case Page::JOIN_LOBBY:
        if (m_joinPage.hasReturnRequest()) {
            m_joinPage.clearConnectionRequest();
            showHomePage(registry);
        }
        // Transition gérée par le système principal via hasJoinRequest()
        break;

    case Page::PARAMETERS:
        if (m_parameterPage.hasReturnRequest()) {
            m_parameterPage.clearRequests();
            showHomePage(registry);
        }
        break;
    case Page::WIN:
        if (m_endPage.hasReturnRequest()) {
            m_endPage.clearConnectionRequest();
            showLobbyPageAfterGame(registry);
        }
        break;
    case Page::LOSE:
        if (m_endPage.hasReturnRequest()) {
            m_endPage.clearConnectionRequest();
            showLobbyPageAfterGame(registry);
        }
        break;
    default:
        break;
    }
}

void Menu::render(GraphicsManager& gfx, Registry& registry)
{
    if (!m_active) {
        return;
    }

    switch (m_currentPage) {
    case Page::HOME:
        m_homePage.render(gfx, registry);
        break;
    case Page::CONNECTION:
        m_connectionPage.render(gfx, registry);
        break;
    case Page::JOIN_LOBBY:
        m_joinPage.render(gfx, registry);
        break;
    case Page::LOBBY:
        m_lobbyPage.render(gfx, registry);
        break;
    case Page::PARAMETERS:
        m_parameterPage.render(gfx, registry);
        break;
    case Page::WIN:
        m_endPage.render(gfx, registry);
        break;
    case Page::LOSE:
        m_endPage.render(gfx, registry);
        break;
    default:
        break;
    }
}

// Getters pour récupérer les données saisies
std::string Menu::getConnectionCode(Registry& registry) const
{
    return m_connectionPage.getConnectionCode(registry);
}

std::string Menu::getJoinCode(Registry& registry) const
{
    return m_joinPage.getConnectionCode(registry);
}

std::string Menu::getUserPseudo(Registry& registry) const
{
    if (m_currentPage == Page::JOIN_LOBBY)
        return m_joinPage.getPseudo();
    return m_homePage.getPseudo(registry);
}

// Vérification des demandes utilisateur
bool Menu::hasConnectionRequest() const
{
    return m_connectionPage.hasConnectionRequest();
}

bool Menu::hasJoinRequest() const
{
    return m_joinPage.hasConnectionRequest();
}

bool Menu::hasCreateRequest() const
{
    return m_homePage.hasCreateRequest();
}

bool Menu::hasLobbyRequest() const
{
    return m_lobbyPage.hasRequest();
}

void Menu::clearAllRequests()
{
    m_connectionPage.clearConnectionRequest();
    m_joinPage.clearConnectionRequest();
    m_homePage.clearRequests();
    m_lobbyPage.clearRequests();
    m_parameterPage.clearRequests();
}

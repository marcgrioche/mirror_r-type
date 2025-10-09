/*
** LobbyMenu.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 9:48:35 PM 2025 jojo
** Last update Thu Oct 8 1:24:54 PM 2025 jojo
*/

#include "LobbyMenu.hpp"
#include "entities/button/CreateButton.hpp"
#include "entities/textbox/TextBox.hpp"
#include <iostream>

LobbyMenu::LobbyMenu()
{
    setupEventHandlers();
}

LobbyMenu::~LobbyMenu() = default;

void LobbyMenu::show(Registry& registry)
{
    if (!m_visible) {
        createEntities(registry);
        m_visible = true;
        clearRequests();
        m_isReturningFromGame = false;
    }
}

void LobbyMenu::showAfterGameEnd(Registry& registry)
{
    if (!m_visible) {
        createEntities(registry);
        m_visible = true;
        clearRequests();
        m_isReturningFromGame = true;
    }
}

void LobbyMenu::hide(Registry& registry)
{
    if (m_visible) {
        destroyEntities(registry);
        m_visible = false;
    }
}

void LobbyMenu::createEntities(Registry& registry)
{
    // TextBoxInput pour le code de connexion
    // m_textBoxEntity = factories::createTextBoxInput(registry,
    //     "Enter Lobby code...", 300.0f, 250.0f, 16, { 255, 255, 255, 255 });

    // Bouton Connect
    m_connectButtonEntity = factories::createButton(registry,
        320.0f, 320.0f, 160.0f, 50.0f, "connect_to_server", true);

    m_returnButtonEntity = factories::createButton(registry,
        320.0f, 380.0f, 160.0f, 50.0f, "return_to_home", true);

    m_connectTextBoxEntity = factories::createTextBox(registry,
        "READY", 320.0f, 320.0f, 16, { 255, 0, 0, 0 });
}

void LobbyMenu::destroyEntities(Registry& registry)
{
    // registry.kill_entity(m_textBoxEntity);
    registry.kill_entity(m_connectButtonEntity);
    registry.kill_entity(m_returnButtonEntity);
    registry.kill_entity(m_connectTextBoxEntity);
}

void LobbyMenu::setupEventHandlers()
{
    auto& eventMgr = EventManager::getInstance();

    eventMgr.subscribe(EventType::BUTTON_CLICK, [this](const GameEvent& event) {
        if (event.data == "connect_to_server" && m_visible) {
            m_LobbyRequested = true;
        }
    });

    eventMgr.subscribe(EventType::BUTTON_CLICK, [this](const GameEvent& event) {
        if (event.data == "return_to_home" && m_visible) {
            m_returnRequested = true;
        }
    });
}

void LobbyMenu::update(Registry& registry, float deltaTime)
{
    if (!m_visible)
        return;
    // textBoxInputUpdateSystem(registry, deltaTime);
    buttonSystem(registry);
}

void LobbyMenu::handleEvent(Registry& registry, const SDL_Event& event)
{
    if (!m_visible)
        return;
    // textBoxInputSystem(registry, event);
}

void LobbyMenu::render(GraphicsManager& gfx, Registry& registry)
{
    if (!m_visible)
        return;

    SDL_Renderer* renderer = gfx.getRenderer();

    // Ajoute un fond comme dans les autres menus
    SDL_SetRenderDrawColor(renderer, 20, 25, 40, 255); // Fond sombre
    SDL_Rect fullBg = { 0, 0, 800, 600 };
    SDL_RenderFillRect(renderer, &fullBg);

    // Boîte du menu
    SDL_SetRenderDrawColor(renderer, 40, 45, 60, 255);
    SDL_Rect menuBox = { 250, 200, 300, 200 };
    SDL_RenderFillRect(renderer, &menuBox);

    // Bordure
    SDL_SetRenderDrawColor(renderer, 100, 120, 150, 255);
    SDL_RenderDrawRect(renderer, &menuBox);

    // Rendu des composants
    // drawTextBoxInput(gfx, registry, m_textBoxEntity);
    drawButton(gfx, registry, m_connectButtonEntity);
    drawTextBox(gfx, registry, m_connectTextBoxEntity);
    drawButton(gfx, registry, m_returnButtonEntity);
}

bool LobbyMenu::hasRequest() const
{
    return m_LobbyRequested;
}

bool LobbyMenu::hasReturnRequest() const
{
    return m_returnRequested;
}

void LobbyMenu::clearRequests()
{
    m_LobbyRequested = false;
    m_returnRequested = false;
}

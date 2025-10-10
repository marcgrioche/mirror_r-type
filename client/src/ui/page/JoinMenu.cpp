/*
** JoinMenu.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 5:46:09 PM 2025 jojo
** Last update Thu Oct 8 1:18:14 PM 2025 jojo
*/

#include "ui/page/JoinMenu.hpp"
#include "entities/button/CreateButton.hpp"
#include "entities/textbox/TextBox.hpp"
#include "entities/textbox/TextBoxInput.hpp"
#include "Config.hpp"
#include <iostream>

JoinMenu::JoinMenu()
{
    setupEventHandlers();
}

JoinMenu::~JoinMenu() = default;

void JoinMenu::show(Registry& registry)
{
    if (!m_visible) {
        createEntities(registry);
        m_visible = true;
        clearConnectionRequest();
    }
}

void JoinMenu::hide(Registry& registry)
{
    if (m_visible) {
        destroyEntities(registry);
        m_visible = false;
    }
}

void JoinMenu::createEntities(Registry& registry)
{
    // TextBoxInput pour le code de connexion
    m_textBoxEntity = factories::createTextBoxInput(registry,
        "Enter Lobby code...", 300.0f, 250.0f, 16, { 255, 255, 255, 255 });

    // Bouton Connect
    m_connectButtonEntity = factories::createButton(registry,
        320.0f, 320.0f, 160.0f, 50.0f, "connect_to_lobby", true);

    m_returnButtonEntity = factories::createButton(registry,
        320.0f, 380.0f, 160.0f, 50.0f, "return_to_home", true);

    m_textBoxConnectEntity = factories::createTextBox(registry,
        "CONNECT", 320.0f, 320.0f, 16, { 255, 0, 0, 0 });
}

void JoinMenu::destroyEntities(Registry& registry)
{
    registry.kill_entity(m_textBoxEntity);
    registry.kill_entity(m_connectButtonEntity);
    registry.kill_entity(m_textBoxConnectEntity);
    registry.kill_entity(m_returnButtonEntity);
}

void JoinMenu::setupEventHandlers()
{
    auto& eventMgr = EventManager::getInstance();

    eventMgr.subscribe(EventType::BUTTON_CLICK, [this](const GameEvent& event) {
        if (event.data == "connect_to_lobby" && m_visible) {
            m_connectionRequested = true;
        }
    });
    eventMgr.subscribe(EventType::BUTTON_CLICK, [this](const GameEvent& event) {
        if (event.data == "return_to_home" && m_visible) {
            m_returnRequested = true;
        }
    });
}

void JoinMenu::update(Registry& registry, float deltaTime)
{
    if (!m_visible)
        return;
    textBoxInputUpdateSystem(registry, deltaTime);
    buttonSystem(registry);
}

void JoinMenu::handleEvent(Registry& registry, const SDL_Event& event)
{
    if (!m_visible)
        return;
    textBoxInputSystem(registry, event);
}

void JoinMenu::render(GraphicsManager& gfx, Registry& registry)
{
    if (!m_visible)
        return;

    SDL_Renderer* renderer = gfx.getRenderer();

    // Fond semi-transparent
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect overlay = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &overlay);

    // Boîte du menu
    SDL_SetRenderDrawColor(renderer, 40, 45, 60, 255);
    SDL_Rect menuBox = { 250, 200, 300, 200 };
    SDL_RenderFillRect(renderer, &menuBox);

    // Bordure
    SDL_SetRenderDrawColor(renderer, 100, 120, 150, 255);
    SDL_RenderDrawRect(renderer, &menuBox);

    // Rendu des composants
    drawTextBoxInput(gfx, registry, m_textBoxEntity);
    drawButton(gfx, registry, m_connectButtonEntity);
    drawTextBox(gfx, registry, m_textBoxConnectEntity);
    drawButton(gfx, registry, m_returnButtonEntity);
}

std::string JoinMenu::getConnectionCode(Registry& registry) const
{
    if (!m_visible || !registry.has<TextBoxInput>(m_textBoxEntity)) {
        return "";
    }

    const auto& input = registry.get<TextBoxInput>(m_textBoxEntity);
    return input.inputText;
}

bool JoinMenu::hasConnectionRequest() const
{
    return m_connectionRequested;
}

bool JoinMenu::hasReturnRequest() const
{
    return m_returnRequested;
}

void JoinMenu::clearConnectionRequest()
{
    m_connectionRequested = false;
    m_returnRequested = false;
}
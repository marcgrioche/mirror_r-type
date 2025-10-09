/*
** ConnectionMenu.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 5:46:09 PM 2025 jojo
** Last update Thu Oct 8 12:45:52 PM 2025 jojo
*/

#include "ui/page/ConnectionMenu.hpp"
#include "entities/button/CreateButton.hpp"
#include "entities/textbox/TextBox.hpp"
#include "entities/textbox/TextBoxInput.hpp"
#include "Config.hpp"
#include <iostream>

ConnectionMenu::ConnectionMenu()
{
    setupEventHandlers();
}

ConnectionMenu::~ConnectionMenu() = default;

void ConnectionMenu::show(Registry& registry)
{
    if (!m_visible) {
        createEntities(registry);
        m_visible = true;
        m_connectionRequested = false;
    }
}

void ConnectionMenu::hide(Registry& registry)
{
    if (m_visible) {
        destroyEntities(registry);
        m_visible = false;
    }
}

void ConnectionMenu::createEntities(Registry& registry)
{
    // TextBoxInput pour le code de connexion
    m_textBoxEntity = factories::createTextBoxInput(registry,
        "Enter connection code...", 300.0f, 250.0f, 16, { 0, 0, 0, 255 });

    // Bouton Connect
    m_connectButtonEntity = factories::createButton(registry,
        320.0f, 320.0f, 160.0f, 50.0f, "connect_to_server", true);

    m_connectTextBoxEntity = factories::createTextBox(registry, "CONNECT",
        320.0f, 340.0f, 16, { 255, 0, 0, 0 }, TextBox::Alignment::CENTER);
}

void ConnectionMenu::destroyEntities(Registry& registry)
{
    registry.kill_entity(m_textBoxEntity);
    registry.kill_entity(m_connectButtonEntity);
    registry.kill_entity(m_connectTextBoxEntity);
}

void ConnectionMenu::setupEventHandlers()
{
    auto& eventMgr = EventManager::getInstance();

    eventMgr.subscribe(EventType::BUTTON_CLICK, [this](const GameEvent& event) {
        if (event.data == "connect_to_server" && m_visible) {
            m_connectionRequested = true;
        }
    });
}

void ConnectionMenu::update(Registry& registry, float deltaTime)
{
    if (!m_visible)
        return;
    textBoxInputUpdateSystem(registry, deltaTime);
    buttonSystem(registry);
}

void ConnectionMenu::handleEvent(Registry& registry, const SDL_Event& event)
{
    if (!m_visible)
        return;
    textBoxInputSystem(registry, event);
}

void ConnectionMenu::render(GraphicsManager& gfx, Registry& registry)
{
    if (!m_visible)
        return;

    SDL_Renderer* renderer = gfx.getRenderer();

    // Ajoute un fond comme dans les autres menus
    SDL_SetRenderDrawColor(renderer, 20, 25, 40, 255); // Fond sombre
    SDL_Rect fullBg = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &fullBg);

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
    drawTextBox(gfx, registry, m_connectTextBoxEntity);
}

std::string ConnectionMenu::getConnectionCode(Registry& registry) const
{
    if (!m_visible || !registry.has<TextBoxInput>(m_textBoxEntity)) {
        return "";
    }

    const auto& input = registry.get<TextBoxInput>(m_textBoxEntity);
    return input.inputText;
}

bool ConnectionMenu::hasConnectionRequest() const
{
    return m_connectionRequested;
}

void ConnectionMenu::clearConnectionRequest()
{
    m_connectionRequested = false;
}
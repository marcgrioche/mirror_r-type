/*
** HomeMenu.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 6:59:23 PM 2025 jojo
** Last update Thu Oct 8 11:25:56 AM 2025 jojo
*/

#include "ui/page/HomeMenu.hpp"
#include "Config.hpp"
#include "entities/button/CreateButton.hpp"
#include "entities/textbox/TextBox.hpp"
#include "entities/textbox/TextBoxInput.hpp"
#include <iostream>

HomeMenu::HomeMenu()
{
    setupEventHandlers();
}

HomeMenu::~HomeMenu() = default;

void HomeMenu::show(Registry& registry)
{
    if (!m_visible) {
        createEntities(registry);
        m_visible = true;
        m_joinRequested = false;
        m_createRequested = false;
    }
}

void HomeMenu::hide(Registry& registry)
{
    if (m_visible) {
        destroyEntities(registry);
        m_visible = false;
    }
}

void HomeMenu::createEntities(Registry& registry)
{
    // TextBoxInput pour le pseudo
    m_textBoxEntity = factories::createTextBoxInput(registry,
        "Enter your username...", 300.0f, 200.0f, 18, { 255, 255, 255, 255 });

    // Bouton Join
    m_joinButtonEntity = factories::createButton(registry,
        250.0f, 280.0f, 120.0f, 50.0f, "home_join", true);

    // Bouton Create
    m_createButtonEntity = factories::createButton(registry,
        430.0f, 280.0f, 120.0f, 50.0f, "home_create", true);

    m_textBoxCreateEntity = factories::createTextBox(registry,
        "CREATE LOBBY", 430.0f, 280.0f, 16, { 255, 0, 0, 0 });
    m_textBoxJoinEntity = factories::createTextBox(registry,
        "JOIN LOBBY", 250.0f, 280.0f, 16, { 255, 0, 0, 0 });
}

void HomeMenu::destroyEntities(Registry& registry)
{
    registry.kill_entity(m_textBoxEntity);
    registry.kill_entity(m_joinButtonEntity);
    registry.kill_entity(m_createButtonEntity);
    registry.kill_entity(m_textBoxCreateEntity);
    registry.kill_entity(m_textBoxJoinEntity);
}

void HomeMenu::setupEventHandlers()
{
    auto& eventMgr = EventManager::getInstance();

    eventMgr.subscribe(EventType::BUTTON_CLICK, [this](const GameEvent& event) {
        if (!m_visible)
            return;

        if (event.data == "home_join") {
            m_joinRequested = true;
            std::cout << "Home Join requested!" << std::endl;
        } else if (event.data == "home_create") {
            m_createRequested = true;
            std::cout << "Home Create requested!" << std::endl;
        }
    });
}

void HomeMenu::update(Registry& registry, float deltaTime)
{
    if (!m_visible)
        return;
    textBoxInputUpdateSystem(registry, deltaTime);
}

void HomeMenu::handleEvent(Registry& registry, const SDL_Event& event)
{
    if (!m_visible)
        return;
    textBoxInputSystem(registry, event);
}

void HomeMenu::render(GraphicsManager& gfx, Registry& registry)
{
    if (!m_visible)
        return;

    SDL_Renderer* renderer = gfx.getRenderer();

    // Fond complet pour le menu principal
    SDL_SetRenderDrawColor(renderer, 15, 20, 35, 255);
    SDL_Rect fullBg = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &fullBg);

    // Rendu des composants
    drawTextBoxInput(gfx, registry, m_textBoxEntity);
    drawButton(gfx, registry, m_joinButtonEntity);
    drawButton(gfx, registry, m_createButtonEntity);
    drawButton(gfx, registry, m_paramButtonEntity);
    drawTextBox(gfx, registry, m_textBoxCreateEntity);
    drawTextBox(gfx, registry, m_textBoxJoinEntity);
}

std::string HomeMenu::getPseudo(Registry& registry) const
{
    if (!m_visible || !registry.has<TextBoxInput>(m_textBoxEntity)) {
        return "";
    }

    const auto& input = registry.get<TextBoxInput>(m_textBoxEntity);
    return input.inputText;
}

bool HomeMenu::hasJoinRequest() const
{
    return m_joinRequested;
}

bool HomeMenu::hasCreateRequest() const
{
    return m_createRequested;
}

bool HomeMenu::hasParamRequest() const
{
    return m_paramRequested;
}

void HomeMenu::clearRequests()
{
    m_joinRequested = false;
    m_createRequested = false;
    // m_paramRequested;
}
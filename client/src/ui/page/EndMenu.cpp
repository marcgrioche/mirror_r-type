/*
** EndMenuPage.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 8 1:41:51 PM 2025 jojo
** Last update Thu Oct 8 2:15:49 PM 2025 jojo
*/

#include "EndMenu.hpp"
#include "entities/button/CreateButton.hpp"
#include "entities/textbox/TextBox.hpp"
#include "entities/textbox/TextBoxInput.hpp"
#include <iostream>

EndMenuPage::EndMenuPage()
{
    setupEventHandlers();
}

EndMenuPage::~EndMenuPage() = default;

void EndMenuPage::show(Registry& registry, bool state)
{
    m_state = (state == true) ? "WIN" : "LOSE";

    if (!m_visible) {
        createEntities(registry);
        m_visible = true;
        clearConnectionRequest();
    }
}

void EndMenuPage::hide(Registry& registry)
{
    if (m_visible) {
        destroyEntities(registry);
        m_visible = false;
    }
}

void EndMenuPage::createEntities(Registry& registry)
{
    // TextBoxInput pour le code de connexion
    m_textBoxStateEntity = factories::createTextBox(registry,
        m_state, 320.0f, 250.0f, 16, { 255, 255, 255, 255 });

    m_returnButtonEntity = factories::createButton(registry,
        320.0f, 320.0f, 160.0f, 50.0f, "return_to_lobby", true);

    m_textBoxReturnEntity = factories::createTextBox(registry,
        "RETURN", 320.0f, 320.0f, 16, { 255, 0, 0, 0 });
}

void EndMenuPage::destroyEntities(Registry& registry)
{
    registry.kill_entity(m_textBoxStateEntity);
    registry.kill_entity(m_textBoxReturnEntity);
    registry.kill_entity(m_returnButtonEntity);
}

void EndMenuPage::setupEventHandlers()
{
    auto& eventMgr = EventManager::getInstance();

    eventMgr.subscribe(EventType::BUTTON_CLICK, [this](const GameEvent& event) {
        if (event.data == "return_to_lobby" && m_visible) {
            m_returnRequested = true;
        }
    });
}

void EndMenuPage::update(Registry& registry, float deltaTime)
{
    if (!m_visible)
        return;
    textBoxInputUpdateSystem(registry, deltaTime);
    buttonSystem(registry);
}

void EndMenuPage::handleEvent(Registry& registry, const SDL_Event& event)
{
    if (!m_visible)
        return;
    textBoxInputSystem(registry, event);
}

void EndMenuPage::render(GraphicsManager& gfx, Registry& registry)
{
    if (!m_visible)
        return;

    SDL_Renderer* renderer = gfx.getRenderer();

    // Fond semi-transparent
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect overlay = { 0, 0, 800, 600 };
    SDL_RenderFillRect(renderer, &overlay);

    // Boîte du menu
    SDL_SetRenderDrawColor(renderer, 40, 45, 60, 255);
    SDL_Rect menuBox = { 250, 200, 300, 200 };
    SDL_RenderFillRect(renderer, &menuBox);

    // Bordure
    SDL_SetRenderDrawColor(renderer, 100, 120, 150, 255);
    SDL_RenderDrawRect(renderer, &menuBox);

    // Rendu des composants
    drawTextBox(gfx, registry, m_textBoxStateEntity);
    drawButton(gfx, registry, m_returnButtonEntity);
    drawTextBox(gfx, registry, m_textBoxReturnEntity);
}

bool EndMenuPage::hasReturnRequest() const
{
    return m_returnRequested;
}

void EndMenuPage::clearConnectionRequest()
{
    m_returnRequested = false;
}
/*
** EndMenuPage.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 8 1:41:51 PM 2025 jojo
** Last update Thu Oct 15 11:12:42 PM 2025 jojo
*/

#include "EndMenu.hpp"
#include "Config.hpp"
#include "components/Sprite.hpp"
#include "components/SpriteFactory.hpp"
#include "entities/button/CreateButton.hpp"
#include "entities/textbox/TextBox.hpp"
#include "entities/textbox/TextBoxInput.hpp"
#include "render/SpriteRender.hpp"
#include <iostream>

EndMenuPage::EndMenuPage()
{
    setupEventHandlers();
}

EndMenuPage::~EndMenuPage() = default;

void EndMenuPage::show(Registry& registry, bool state)
{
    m_state = (state == true) ? "WIN" : "LOSE";
    // If already visible, rebuild with the new state to update text
    if (m_visible) {
        destroyEntities(registry);
        clearConnectionRequest();
        m_visible = false;
    }
    createEntities(registry);
    m_visible = true;
    clearConnectionRequest();
}

void EndMenuPage::hide(Registry& registry)
{
    if (m_visible) {
        destroyEntities(registry);
        m_visible = false;
        // Reset any pending UI flags so next show starts fresh
        clearConnectionRequest();
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

    Sprite bg = SpriteFactory::createStaticSprite("MenuBackground",
        0, 0, 2480, 2486, 1.0f, 1.0f, 0, 0);

    bg.dstRect = { 0, 0, SCREEN_WIDTH, SCREEN_WIDTH };
    if (bg.frame_width > 0 && bg.frame_height > 0) {
        bg.scale_x = static_cast<float>(SCREEN_WIDTH) / bg.frame_width;
        bg.scale_y = static_cast<float>(SCREEN_WIDTH) / bg.frame_width;
    }

    m_backgroundEntity = registry.create_entity();
    registry.emplace<Position>(m_backgroundEntity, 0.0f, 0.0f);
    registry.add<Sprite>(m_backgroundEntity, bg);
}

void EndMenuPage::destroyEntities(Registry& registry)
{
    registry.kill_entity(m_textBoxStateEntity);
    registry.kill_entity(m_textBoxReturnEntity);
    registry.kill_entity(m_returnButtonEntity);
    registry.kill_entity(m_backgroundEntity);
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
    drawSprite(gfx, registry, m_backgroundEntity);
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
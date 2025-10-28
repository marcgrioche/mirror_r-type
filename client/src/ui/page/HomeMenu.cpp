/*
** HomeMenu.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 6:59:23 PM 2025 jojo
** Last update Wed Oct 21 3:28:56 PM 2025 jojo
*/

#include "ui/page/HomeMenu.hpp"
#include "Config.hpp"
#include "components/Sprite.hpp"
#include "components/SpriteFactory.hpp"
#include "entities/Sprite/CreateAnimateSprite.hpp"
#include "entities/button/CreateButton.hpp"
#include "entities/textbox/TextBox.hpp"

#include "render/SpriteRender.hpp"
#include "render/TextBoxRender.hpp"

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
    m_joinButtonEntity = factories::createButton(registry,
        SCREEN_WIDTH / 2 - 250 + 600, SCREEN_HEIGHT / 2 - 250, 380.0f, 120.0f, "home_join", true, "ButtonMouth", 500, 500, 1200, 1080);

    m_createButtonEntity = factories::createButton(registry,
        SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 - 250, 380.0f, 120.0f, "home_create", true, "ButtonMouth", 500, 500, 1200, 1080);

    m_paramButtonEntity = factories::createButton(registry,
        SCREEN_WIDTH / 2 - 250 - 600, SCREEN_HEIGHT / 2 - 250, 380.0f, 120.0f, "home_param", true, "ButtonMouth", 500, 500, 1200, 1080);

    m_textBoxJoinEntity = factories::createTextBox(registry,
        "JOIN LOBBY", SCREEN_WIDTH / 2 - 120 + 600, SCREEN_HEIGHT / 2 - 25, 40, { 255, 0, 0, 0 });
    m_textBoxCreateEntity = factories::createTextBox(registry,
        "CREATE LOBBY", SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 - 25, 40, { 255, 0, 0, 0 });
    m_textBoxParamEntity = factories::createTextBox(registry,
        "PARAMETER", SCREEN_WIDTH / 2 - 120 - 600, SCREEN_HEIGHT / 2 - 25, 40, { 255, 0, 0, 0 });

    m_backgroundMenu.reload(registry, 75, { m_joinButtonEntity, m_createButtonEntity, m_paramButtonEntity });
}

void HomeMenu::destroyEntities(Registry& registry)
{
    registry.kill_entity(m_joinButtonEntity);
    registry.kill_entity(m_createButtonEntity);
    registry.kill_entity(m_paramButtonEntity);

    registry.kill_entity(m_textBoxCreateEntity);
    registry.kill_entity(m_textBoxJoinEntity);
    registry.kill_entity(m_textBoxParamEntity);

    m_backgroundMenu.destroy(registry);
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
        } else if (event.data == "home_param") {
            std::cout << "Home Param requested!" << std::endl;
            m_paramRequested = true;
        }
    });
}

void HomeMenu::update(Registry& registry, float deltaTime)
{
    if (!m_visible)
        return;
    (void)registry;
    (void)deltaTime;
}

void HomeMenu::handleEvent(Registry& registry, const SDL_Event& event)
{
    if (!m_visible)
        return;
    (void)registry;
    (void)event;
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
    m_backgroundMenu.draw(gfx, registry);

    drawButton(gfx, registry, m_joinButtonEntity);
    drawButton(gfx, registry, m_createButtonEntity);
    drawButton(gfx, registry, m_paramButtonEntity);
    // drawButton(gfx, registry, m_paramButtonEntity);

    drawTextBox(gfx, registry, m_textBoxCreateEntity);
    drawTextBox(gfx, registry, m_textBoxJoinEntity);
    drawTextBox(gfx, registry, m_textBoxParamEntity);
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
    m_paramRequested = false;
}

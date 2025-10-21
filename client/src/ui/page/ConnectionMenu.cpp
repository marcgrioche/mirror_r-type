/*
** ConnectionMenu.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 5:46:09 PM 2025 jojo
** Last update Wed Oct 21 1:22:37 AM 2025 jojo
*/

#include "ConnectionMenu.hpp"
#include "Config.hpp"
#include "components/Sprite.hpp"
#include "components/SpriteFactory.hpp"
#include "entities/Sprite/CreateAnimateSprite.hpp"
#include "entities/button/CreateButton.hpp"
#include "entities/eyes/CreateEye.hpp"
#include "entities/textbox/TextBox.hpp"
#include "entities/textbox/TextBoxInput.hpp"
#include "managers/ResourceManager.hpp"
#include "render/EyeRender.hpp"
#include "render/SpriteRender.hpp"
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
        "Enter connection code...", WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 300, 30, { 255, 0, 0, 0 });

    m_textBoxSpriteEntity = factories::createSprite(registry, "zoneText",
        WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 500, 400, 400, 500, 500);

    // Bouton Connect
    m_connectButtonEntity = factories::createButton(registry,
        WINDOW_WIDTH / 2 - 250, WINDOW_HEIGHT / 2 - 50, 380.0f, 120.0f, "connect_to_server", true, "ButtonMouth", 500, 500, 1200, 1080);

    m_connectTextBoxEntity = factories::createTextBox(registry, "CONNECT",
        WINDOW_WIDTH / 2 - 120, WINDOW_HEIGHT / 2 + 180, 50, { 255, 00, 00, 00 }, TextBox::Alignment::CENTER);

    bg.reload(registry, 50);
}

void ConnectionMenu::destroyEntities(Registry& registry)
{
    registry.kill_entity(m_textBoxEntity);
    registry.kill_entity(m_connectButtonEntity);
    registry.kill_entity(m_connectTextBoxEntity);
    registry.kill_entity(m_textBoxSpriteEntity);

    bg.destroy(registry);
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

    bg.draw(gfx, registry);
    // Rendu des composants
    drawSprite(gfx, registry, m_textBoxSpriteEntity);
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
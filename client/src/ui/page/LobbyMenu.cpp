/*
** LobbyMenu.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 9:48:35 PM 2025 jojo
** Last update Thu Oct 15 11:14:04 PM 2025 jojo
*/

#include "LobbyMenu.hpp"
#include "Config.hpp"
#include "components/Sprite.hpp"
#include "components/SpriteFactory.hpp"
#include "entities/button/CreateButton.hpp"
#include "entities/textbox/TextBox.hpp"
#include "render/SpriteRender.hpp"
#include <iostream>

LobbyMenu::LobbyMenu()
{
    setupEventHandlers();
}

LobbyMenu::~LobbyMenu() = default;

void LobbyMenu::show(Registry& registry, int lobbyId)
{
    if (!m_visible) {
        createEntities(registry, lobbyId);
        m_visible = true;
        clearRequests();
        m_isReturningFromGame = false;
    }
}

void LobbyMenu::showAfterGameEnd(Registry& registry)
{
    if (!m_visible) {
        createEntities(registry, 0);
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
        // Reset pending requests when leaving lobby to avoid sticky state
        clearRequests();
    }
}

void LobbyMenu::createEntities(Registry& registry, int lobbyId)
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

    m_textBoxLobbyEntity = factories::createTextBox(registry,
        std::to_string(lobbyId), 320.0f, 220.0f, 16, { 255, 255, 255, 255 });

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

void LobbyMenu::destroyEntities(Registry& registry)
{
    // registry.kill_entity(m_textBoxEntity);
    registry.kill_entity(m_connectButtonEntity);
    registry.kill_entity(m_returnButtonEntity);
    registry.kill_entity(m_connectTextBoxEntity);
    registry.kill_entity(m_textBoxLobbyEntity);
    registry.kill_entity(m_backgroundEntity);
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
    (void)deltaTime;
    if (!m_visible)
        return;
    // textBoxInputUpdateSystem(registry, deltaTime);
    buttonSystem(registry);
}

void LobbyMenu::handleEvent(Registry& registry, const SDL_Event& event)
{
    (void)registry;
    (void)event;
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
    // drawTextBoxInput(gfx, registry, m_textBoxEntity);
    drawSprite(gfx, registry, m_backgroundEntity);
    drawButton(gfx, registry, m_connectButtonEntity);
    drawTextBox(gfx, registry, m_connectTextBoxEntity);
    drawButton(gfx, registry, m_returnButtonEntity);
    drawTextBox(gfx, registry, m_textBoxLobbyEntity);
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

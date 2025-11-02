/*
** JoinMenu.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 5:46:09 PM 2025 jojo
** Last update Thu Oct 15 11:13:45 PM 2025 jojo
*/

#include "ui/page/JoinMenu.hpp"
#include "Config.hpp"
#include "components/Sprite.hpp"
#include "components/SpriteFactory.hpp"
#include "entities/button/CreateButton.hpp"
#include "entities/textbox/TextBox.hpp"
#include "entities/textbox/TextBoxInput.hpp"
#include "render/SpriteRender.hpp"
#include <iostream>

#include "entities/Sprite/CreateAnimateSprite.hpp"

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
    m_entities["lobby_code_input"] = factories::createTextBoxInput(registry,
        "Enter Lobby code...",
        WINDOW_WIDTH / 2.f - 200.0f,
        WINDOW_HEIGHT / 2.f - 300.0f,
        30,
        { 255, 255, 255, 255 });
    m_entities["lobby_code_input_bg"] = factories::createSprite(registry, "zoneText",
        WINDOW_WIDTH / 2.f - 220.0f,
        WINDOW_HEIGHT / 2.f - 500.0f,
        400, 400, 500, 500);

    // Bouton Connect
    m_entities["connect_to_lobby_btn"] = factories::createButton(registry,
        WINDOW_WIDTH / 2.f - 630.0f,
        WINDOW_HEIGHT / 2.f - 50.0f, 380.0f, 120.0f,
        "connect_to_lobby", true, "ButtonMouth", 500, 500, 1200, 1080);
    m_entities["connect_textbox"] = factories::createTextBox(registry,
        "CONNECT",
        WINDOW_WIDTH / 2.f - 500.f,
        WINDOW_HEIGHT / 2.f + 180.0f,
        30,
        { 255, 0, 0, 0 });

    // Bouton Return
    m_entities["return_to_home_btn"] = factories::createButton(registry,
        WINDOW_WIDTH / 2.f + 100.0f,
        WINDOW_HEIGHT / 2 - 50.0f,
        380.0f, 120.0f,
        "return_to_home", true, "ButtonMouth", 500, 500, 1200, 1080);
    m_entities["return_texbox"] = factories::createTextBox(registry,
        "RETURN",
        WINDOW_WIDTH / 2.f + 230.f,
        WINDOW_HEIGHT / 2.f + 180.0f,
        30,
        { 255, 0, 0, 0 });

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

void JoinMenu::destroyEntities(Registry& registry)
{
    for (const auto& pair : m_entities) {
        registry.kill_entity(pair.second);
    }
    registry.kill_entity(m_backgroundEntity);
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
    drawSprite(gfx, registry, m_backgroundEntity);
    drawSprite(gfx, registry, m_entities["lobby_code_input_bg"]);
    drawTextBoxInput(gfx, registry, m_entities["lobby_code_input"]);
    drawButton(gfx, registry, m_entities["connect_to_lobby_btn"]);
    drawTextBox(gfx, registry, m_entities["connect_textbox"]);
    drawButton(gfx, registry, m_entities["return_to_home_btn"]);
    drawTextBox(gfx, registry, m_entities["return_texbox"]);
}

std::string JoinMenu::getConnectionCode(Registry& registry) const
{
    if (!m_visible)
        return "";

    auto it = m_entities.find("lobby_code_input");
    if (it == m_entities.end())
        return "";

    const Entity lobbyEntity = it->second;
    if (!registry.has<TextBoxInput>(lobbyEntity))
        return "";

    const auto& input = registry.get<TextBoxInput>(lobbyEntity);
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

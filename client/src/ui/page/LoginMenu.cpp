#include "LoginMenu.hpp"
#include "Config.hpp"
#include "entities/button/CreateButton.hpp"
#include "entities/button/CreateButtonInfo.hpp"
#include "entities/textbox/TextBox.hpp"
#include "entities/textbox/TextBoxInput.hpp"
#include "managers/EventManager.hpp"
#include "render/ButtonRender.hpp"
#include "render/TextBoxInputRender.hpp"

LoginMenu::LoginMenu()
{
    setupEventHandlers();
}

void LoginMenu::createEntities(Registry& registry)
{
    // TextBoxInput for the username
    m_entities["username_input"] = factories::createTextBoxInput(registry,
        "Enter your username...", WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 300, 30, { 255, 0, 0, 0 });
    m_entities["password_input"] = factories::createTextBoxInput(registry,
        "Enter your password...", WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 300, 30, { 255, 0, 0, 0 });

    // Button login
    m_entities["login_button"] = factories::createButton(registry,
        WINDOW_WIDTH / 2 - 250, WINDOW_HEIGHT / 2 - 50, 380.0f, 120.0f, "login", true, "ButtonMouth", 500, 500, 1200, 1080);
    m_entities["login_textbox"] = factories::createTextBox(registry, "LOGIN",
        WINDOW_WIDTH / 2 - 120, WINDOW_HEIGHT / 2 + 180, 50, { 255, 00, 00, 00 }, TextBox::Alignment::CENTER);

    m_bg.reload(registry, 55, { m_entities["login_button"] });
}

void LoginMenu::setupEventHandlers()
{
    auto& eventMgr = EventManager::getInstance();

    eventMgr.subscribe(EventType::BUTTON_CLICK, [this](const GameEvent& event) {
        if (event.data == "login" && m_visible) {
            m_loginRequested = true;
        }
    });
}

void LoginMenu::render(GraphicsManager& gfx, Registry& registry)
{
    if (!m_visible)
        return;

    m_bg.draw(gfx, registry);
    drawTextBoxInput(gfx, registry, m_entities["username_input"]);
    drawTextBoxInput(gfx, registry, m_entities["password_input"]);
    drawButton(gfx, registry, m_entities["login_button"]);
    drawTextBox(gfx, registry, m_entities["login_textbox"]);
}

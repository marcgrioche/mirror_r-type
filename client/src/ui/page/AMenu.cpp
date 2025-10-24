#include "AMenu.hpp"
#include "ButtonSystem.hpp"
#include "TextBoxInputSystem.hpp"

void AMenu::show(Registry& registry)
{
    if (!m_visible) {
        createEntities(registry);
        m_visible = true;
        resetOtherOptions();
    }
}

void AMenu::hide(Registry& registry)
{
    if (m_visible) {
        destroyEntities(registry);
        m_visible = false;
        // resetOtherOptions();
    }
}

void AMenu::update(Registry& registry, const float deltaTime)
{
    if (!m_visible)
        return;
    textBoxInputUpdateSystem(registry, deltaTime);
    buttonSystem(registry);
}

void AMenu::destroyEntities(Registry& registry)
{
    for (const auto& pair : m_entities) {
        registry.kill_entity(pair.second);
    }
    m_bg.destroy(registry);
}

void AMenu::handleEvent(Registry& registry, const SDL_Event& event)
{
    if (!m_visible)
        return;
    textBoxInputSystem(registry, event);
}

bool AMenu::isVisible() const
{
    return m_visible;
}

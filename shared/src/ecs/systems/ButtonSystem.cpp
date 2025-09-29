#include "ButtonSystem.hpp"
#include "EventManager.hpp"
#include "components/AllComponents.hpp"

void buttonSystem(Registry& registry)
{
    // Récupère état souris (comme dans InputManager::updateInputs)
    int mx, my;
    Uint32 mouseState = SDL_GetMouseState(&mx, &my);
    bool mousePressed = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;

    auto& eventMgr = EventManager::getInstance();

    auto view = registry.view<Button, Position, Hitbox>();
    for (auto [button, pos, hitbox] : view) {
        Entity e = /* récupère l'entité de la view */;

        // Test point-in-rect
        bool hovered = (mx >= pos.x && mx <= pos.x + hitbox.width && my >= pos.y && my <= pos.y + hitbox.height);

        // Détection clic
        if (hovered && !button.was_pressed && mousePressed && button.interactable) {
            button.was_pressed = true;
        } else if (button.was_pressed && !mousePressed) {
            // Relâchement = clic validé
            if (hovered) {
                eventMgr.emitButtonClick(e, button.action_id);
            }
            button.was_pressed = false;
        }

        button.is_hovered = hovered;
    }
}
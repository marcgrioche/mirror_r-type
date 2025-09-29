/*
** ButtonSystem.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Sep 29 1:26:02 PM 2025 jojo
** Last update Tue Sep 29 1:26:04 PM 2025 jojo
*/

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
    for (auto it = view.begin(); it != view.end(); ++it) {
        Entity e = it.entity();

        // require Position + Hitbox + Button
        if (!registry.has<Position>(e) || !registry.has<Hitbox>(e) || !registry.has<Button>(e))
            continue;

        // get component references
        Button& button = registry.get<Button>(e);
        Position& pos = registry.get<Position>(e);
        Hitbox& hitbox = registry.get<Hitbox>(e);

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
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
#include "../../managers/EventManager.hpp"
#include "components/AllComponents.hpp"
#include <SDL2/SDL.h>

void buttonSystem(Registry& registry)
{
    // Edge-triggered left-click handling, consumed once per press
    static bool prevMousePressed = false;          // state at previous frame
    static bool clickConsumedForThisPress = false; // only one button per press

    int mx = 0, my = 0;
    const Uint32 mouseState = SDL_GetMouseState(&mx, &my);
    const bool mousePressed = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;

    const bool justPressed = mousePressed && !prevMousePressed;
    const bool justReleased = !mousePressed && prevMousePressed;

    if (justReleased) {
        // reset consumption for next press
        clickConsumedForThisPress = false;
    }

    auto& eventMgr = EventManager::getInstance();

    auto view = registry.view<Button, Position, Hitbox>();
    for (auto it = view.begin(); it != view.end(); ++it) {
        Entity e = it.entity();

        if (!registry.has<Position>(e) || !registry.has<Hitbox>(e) || !registry.has<Button>(e))
            continue;

        Button& button = registry.get<Button>(e);
        Position& pos = registry.get<Position>(e);
        Hitbox& hitbox = registry.get<Hitbox>(e);

        const bool hovered = (mx >= pos.x && mx <= pos.x + hitbox.width && my >= pos.y && my <= pos.y + hitbox.height);
        button.is_hovered = hovered && button.interactable;

        // Fire only on the press edge and only once per press
        if (button.interactable && hovered && justPressed && !clickConsumedForThisPress) {
            button.was_pressed = true; // UI code will consume and reset this
            eventMgr.emitButtonClick(e, button.action_id);
            clickConsumedForThisPress = true;
        }
    }

    prevMousePressed = mousePressed;
}
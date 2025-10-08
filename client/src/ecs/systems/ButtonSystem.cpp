/*
** ButtonSystem.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Sep 29 1:26:02 PM 2025 jojo
** Last update Wed Oct 7 3:13:55 PM 2025 jojo
*/

#include "ButtonSystem.hpp"
#include "../../managers/EventManager.hpp"
#include "components/Button.hpp"
#include "components/Hitbox.hpp"
#include "components/Position.hpp"
#include <SDL2/SDL.h>
#include <algorithm>

void buttonSystem(Registry& registry)
{
    static bool prevMousePressed = false;
    static bool clickConsumedForThisPress = false;

    int mx = 0, my = 0;
    const Uint32 mouseState = SDL_GetMouseState(&mx, &my);
    const bool mousePressed = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;

    const bool justPressed = mousePressed && !prevMousePressed;
    const bool justReleased = !mousePressed && prevMousePressed;

    if (justReleased) {
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

        if (button.interactable && hovered && justPressed && !clickConsumedForThisPress) {
            button.was_pressed = true;
            eventMgr.emitButtonClick(e, button.action_id);
            clickConsumedForThisPress = true;
        }
    }

    prevMousePressed = mousePressed;
}
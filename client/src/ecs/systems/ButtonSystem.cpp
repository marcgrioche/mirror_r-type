/*
** ButtonSystem.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Sep 29 1:26:02 PM 2025 jojo
** Last update Fri Oct 16 1:41:56 AM 2025 jojo
*/

#include "ButtonSystem.hpp"
#include "../../managers/EventManager.hpp"
#include "components/Button.hpp"
#include "components/Hitbox.hpp"
#include "components/Position.hpp"
#include "components/Sprite.hpp"
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

        int hx = static_cast<int>(pos.v.x + hitbox.offset_x);
        int hy = static_cast<int>(pos.v.y + hitbox.offset_y);
        int hw = static_cast<int>(hitbox.width);
        int hh = static_cast<int>(hitbox.height);

        const bool hovered = (mx >= hx && mx <= hx + hw && my >= hy && my <= hy + hh);
        button.is_hovered = hovered && button.interactable;
        if (registry.has<Sprite>(e)) {
            Sprite& sprite = registry.get<Sprite>(e);
            if (button.is_hovered)
                sprite.current_frameX = 1;
            else
                sprite.current_frameX = 0;
        }

        if (button.interactable && hovered && justPressed && !clickConsumedForThisPress) {
            button.was_pressed = true;
            eventMgr.emitButtonClick(e, button.action_id);
            clickConsumedForThisPress = true;
        }
    }

    prevMousePressed = mousePressed;
}

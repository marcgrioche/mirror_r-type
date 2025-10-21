/*
** CreateButton.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/entities/button
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Sep 29 1:41:01 PM 2025 jojo
** Last update Fri Oct 16 1:15:48 AM 2025 jojo
*/

#include "CreateButton.hpp"
#include "components/Sprite.hpp"
#include "components/SpriteFactory.hpp"

namespace factories {

Entity createButton(Registry& registry, float x, float y, float width, float height,
    const std::string& actionId, bool interactable, const std::string& textureId, float xS, float yS, float wS, float hS)
{
    Entity button = registry.create_entity();

    registry.emplace<Position>(button, x, y);
    registry.emplace<Button>(button, actionId, false, false, interactable);

    if (!textureId.empty()) {
        Sprite sprite = SpriteFactory::createAnimatedSprite(
            textureId, // texture id (or path if you use path-as-id)
            wS, hS, // frame width, frame height in the sprite sheet
            2, // total frames
            0.2f, // frame duration (seconds)
            xS / wS, // scaleX to fit requested width
            yS / hS, // scaleY to fit requested height
            0, 0 // offsetX, offsetY
        );

        // Ensure the dstRect matches the button position/size
        sprite.dstRect = { static_cast<int>(x), static_cast<int>(y), static_cast<int>(xS), static_cast<int>(yS) };

        registry.add<Sprite>(button, sprite);
        registry.emplace<Hitbox>(button, width, height, 70.0f, 200.0f);
    } else {
        registry.emplace<Hitbox>(button, width, height, 0.0f, 0.0f);
    }

    return button;
}

}
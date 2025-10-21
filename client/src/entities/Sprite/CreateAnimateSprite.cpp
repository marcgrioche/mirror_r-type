/*
** CreateAnimateSprite.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/entities/Sprite
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Oct 20 5:20:11 PM 2025 jojo
** Last update Tue Oct 20 5:55:59 PM 2025 jojo
*/

#include "CreateAnimateSprite.hpp"

namespace factories {
Entity createSprite(Registry& registry, const std::string& textureId, float x, float y,
    float width, float height, float wRect, float hRect, float offsetX, float offsetY,
    int nbrFrame, float frameDuration)
{
    Entity sprite = registry.create_entity();

    registry.emplace<Position>(sprite, x, y);

    Sprite spr = SpriteFactory::createAnimatedSprite(
        textureId, // texture id (or path if you use path-as-id)
        wRect, hRect, // frame width, frame height in the sprite sheet
        nbrFrame, // total frames
        frameDuration, // frame duration (seconds)
        width / wRect, // scaleX to fit requested width
        height / hRect, // scaleY to fit requested height
        offsetX, offsetY // offsets
    );

    spr.dstRect = { static_cast<int>(x), static_cast<int>(y),
        static_cast<int>(width), static_cast<int>(height) };

    registry.add<Sprite>(sprite, spr);

    return sprite;
}
}
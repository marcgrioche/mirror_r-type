/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SpriteManager - Manages sprite creation and attachment to entities
*/

#include "SpriteManager.hpp"
#include "components/Hitbox.hpp"
#include "components/Sprite.hpp"
#include "components/SpriteFactory.hpp"

void SpriteManager::addPlayerSprite(Registry& registry, Entity entity, float posX, float posY)
{
    (void)posX; // Unused parameter, kept for API compatibility
    (void)posY; // Unused parameter, kept for API compatibility

    if (!registry.has<Hitbox>(entity)) {
        return; // Cannot add sprite without hitbox
    }

    Hitbox& hitbox = registry.get<Hitbox>(entity);

    const float SPRITE_WIDTH = 623.0f;
    const float SPRITE_HEIGHT = 623.0f;

    float scale = (hitbox.width < hitbox.height) ? hitbox.width / SPRITE_WIDTH : hitbox.height / SPRITE_HEIGHT;

    float rendered_width = SPRITE_WIDTH * scale;
    float rendered_height = SPRITE_HEIGHT * scale;
    float offset_x = -(rendered_width / 2.0f) + (hitbox.width / 2.0f);
    float offset_y = -(rendered_height / 2.0f) + (hitbox.height / 2.0f);

    Sprite sprite = SpriteFactory::createStaticSprite(
        "player_sprite.png", // texture ID
        0, 0, SPRITE_WIDTH, SPRITE_HEIGHT, // src rect (x, y, w, h) - full image
        scale, // scale to fit hitbox
        offset_x, offset_y // offset to center on entity
    );

    registry.add<Sprite>(entity, sprite);
}

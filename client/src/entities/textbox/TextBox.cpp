/*
** TextBox.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/entities/textbox
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 11:03:28 AM 2025 jojo
** Last update Tue Oct 20 3:51:28 PM 2025 jojo
*/

#include "TextBox.hpp"
#include "Position.hpp"
#include "TextBox.hpp"
#include "components/Sprite.hpp"
#include "components/SpriteFactory.hpp"

namespace factories {

Entity createTextBox(Registry& registry, const std::string& text,
    float x, float y, int fontSize, Color color, TextBox::Alignment alignement,
    const std::string& textureId, float xS, float yS, float wS, float hS)
{
    Entity textBox = registry.create_entity();

    registry.emplace<Position>(textBox, x, y);
    registry.emplace<TextBox>(textBox, text, fontSize, color,
        "client/res/fonts/metali1.ttf", true,
        alignement);

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

        registry.add<Sprite>(textBox, sprite);
    }

    return textBox;
}

}
/*
** TextBoxInput.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/entities/textbox
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 2:31:26 PM 2025 jojo
** Last update Tue Oct 20 7:00:19 PM 2025 jojo
*/

#include "TextBoxInput.hpp"
#include "Hitbox.hpp"
#include "Position.hpp"
#include "components/Sprite.hpp"
#include "components/SpriteFactory.hpp"
#include "components/TextBox.hpp"
#include "components/TextBoxInput.hpp"
#include "managers/ResourceManager.hpp"

namespace factories {

Entity createTextBoxInput(Registry& registry, const std::string& placeholder,
    float x, float y, int fontSize, Color color)
{
    Entity inputBox = registry.create_entity();

    // Position
    registry.emplace<Position>(inputBox, x, y);

    // Hitbox pour détection clic (estimation basée sur taille)
    float estimatedWidth = fontSize * 15; // largeur fixe raisonnable
    float estimatedHeight = fontSize * 1.5f;
    // registry.emplace<Hitbox>(inputBox, estimatedWidth, estimatedHeight, 0.0f, 0.0f);

    // TextBox pour l'affichage
    registry.emplace<TextBox>(inputBox, placeholder, fontSize, color,
        ResourceManager::getInstance().getAssetPath("fonts/OpenSans-Medium.ttf"), true, TextBox::Alignment::CENTER);

    // Component input
    registry.emplace<TextBoxInput>(
        inputBox,
        "",
        placeholder,
        false,
        true,
        static_cast<size_t>(20),
        static_cast<size_t>(0),
        0.0f,
        true,
        color,
        color);

    registry.emplace<Hitbox>(inputBox, estimatedWidth, estimatedHeight, 0.0f, 0.0f);

    return inputBox;
}

}

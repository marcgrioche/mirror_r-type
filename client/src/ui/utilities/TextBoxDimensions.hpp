#pragma once
#include <SDL2/SDL_rect.h>

#include "Registry.hpp"

/**
 * This function calculates and returns the dimensions of a TextBox component
 * associated with a given entity in the ECS registry.
 * @param registry The entire ECS registry
 * @param entity The entity containing the TextBox and Position components
 * @return The SDL_Rect representing the dimensions of the TextBox
 */
SDL_Rect getTextBoxDimensions(Registry& registry, const Entity& entity);

/**
 * @brief Calculates the X offset needed to center an element of given rectangle
 *        within an element with a given width.
 * @param rect The SDL_Rect representing the bounding rectangle
 * @param width The width of the element in which to center
 * @return The X offset for centering the element
 */
float getXOffset(const SDL_Rect& rect, float width);

/**
 * @brief Calculates the Y offset needed to center an element of given rectangle
 *        within an element with a given height.
 * @param rect The SDL_Rect representing the bounding rectangle
 * @param height The height of the element in which to center
 * @return The Y offset for centering the element
 */
float getYOffset(const SDL_Rect& rect, float height);

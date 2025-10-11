/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Level
*/

#pragma once

/**
 * @brief Represents a game level with physics properties
 */
class Level {
private:
    float gravity = 1.0f; /**< Gravity value for the level */

public:
    /**
     * @brief Constructs a Level instance
     */
    Level(/* args */);

    /**
     * @brief Destroys the Level instance
     */
    ~Level();
};

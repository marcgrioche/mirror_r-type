/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Platform Components - Different types of platform behaviors
*/

#pragma once

/**
 * @brief Empty tag component for platforms that can be passed through from below
 */
struct BottomPassPlatform { };

/**
 * @brief Empty tag component for platforms that cannot be passed through
 */
struct NoPassPlatform { };

/**
 * @brief Component for platforms that move horizontally between two points
 * @param min_x Leftmost X position boundary
 * @param max_x Rightmost X position boundary
 * @param speed Movement speed in pixels per second
 * @param movingRight True if currently moving right, false if moving left
 */
struct MovingPlatform {
    float min_x, max_x;
    float speed;
    bool movingRight;
};

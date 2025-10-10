/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** ParallaxState component - Stores persistent parallax background scroll offsets
*/

#pragma once

/**
 * @brief Component that stores the current scroll offsets for parallax background layers
 * This ensures the background scrolls continuously instead of resetting each frame
 */
struct ParallaxState {
    float skyOffset; // Sky layer scroll offset
    float downLayerOffset; // Down layer scroll offset
    float middleLayerOffset; // Middle layer scroll offset
    float lightOffset; // Light layer scroll offset
    float topLayerOffset; // Top layer scroll offset
};

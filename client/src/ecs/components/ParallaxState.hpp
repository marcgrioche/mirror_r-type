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
 * @param skyOffset Current scroll offset for the sky background layer
 * @param downLayerOffset Current scroll offset for the down background layer
 * @param middleLayerOffset Current scroll offset for the middle background layer
 * @param lightOffset Current scroll offset for the light background layer
 * @param topLayerOffset Current scroll offset for the top background layer
 */
struct ParallaxState {
    float skyOffset; // Sky layer scroll offset
    float downLayerOffset; // Down layer scroll offset
    float middleLayerOffset; // Middle layer scroll offset
    float lightOffset; // Light layer scroll offset
    float topLayerOffset; // Top layer scroll offset
};

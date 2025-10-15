/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Display Configuration Presets
*/

#pragma once

//  DO NOT CHANGE unless you want to redesign the entire game layout
constexpr int GAME_LOGICAL_WIDTH = 1920;
constexpr int GAME_LOGICAL_HEIGHT = 1080;

// Backwards compatibility
constexpr int SCREEN_WIDTH = GAME_LOGICAL_WIDTH;
constexpr int SCREEN_HEIGHT = GAME_LOGICAL_HEIGHT;

enum class DisplayMode {
    WINDOWED,
    FULLSCREEN_DESKTOP,
    FULLSCREEN_EXCLUSIVE
};

constexpr bool MAINTAIN_ASPECT_RATIO = true;
constexpr bool INTEGER_SCALING = false;

constexpr int RENDER_WIDTH = 1920;
constexpr int RENDER_HEIGHT = 1080;
constexpr int WINDOW_WIDTH = 1920;
constexpr int WINDOW_HEIGHT = 1080;
constexpr DisplayMode DISPLAY_MODE = DisplayMode::WINDOWED;

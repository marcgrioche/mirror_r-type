/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Frequency
*/

#pragma once

struct Frequency {
    float seconds = 1; //happends every n seconds
    float lastTime = seconds; //store the last time the frequency have been reached
};

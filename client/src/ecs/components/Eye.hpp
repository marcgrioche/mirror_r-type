/*
** Eye.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/components
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Oct 20 8:29:45 PM 2025 jojo
** Last update Wed Oct 21 3:07:30 PM 2025 jojo
*/

#pragma once
#include "Entity.hpp"

/**
 * Eye component holds references to the two child entities (outline and pupil)
 * and parameters for pupil movement.
 */
struct Eye {
    Entity outlineEntity;
    Entity pupilEntity;
    // center offset inside the eye local box (usually width/2, height/2)
    float center_offset_x = 0.0f;
    float center_offset_y = 0.0f;
    // max distance the pupil can move from the center (in pixels)
    float pupil_radius = 8.0f;
    // pupil size (used to correctly center the pupil sprite)
    int pupil_w = 0;
    int pupil_h = 0;
};

/*
** EyeRender.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/render
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Oct 20 8:53:07 PM 2025 jojo
** Last update Tue Oct 20 11:10:02 PM 2025 jojo
*/

#include "EyeRender.hpp"

void drawEye(GraphicsManager& gfx, Registry& registry, Entity entity)
{
    if (!registry.has<Position>(entity) || !registry.has<Eye>(entity))
        return;

    Eye spr = registry.get<Eye>(entity);
    drawSprite(gfx, registry, spr.pupilEntity);
    drawSprite(gfx, registry, spr.outlineEntity);
}
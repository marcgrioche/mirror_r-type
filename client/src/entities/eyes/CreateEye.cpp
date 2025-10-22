/*
** CreateEye.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/entities/eyes
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Oct 20 8:31:03 PM 2025 jojo
** Last update Wed Oct 21 12:33:59 AM 2025 jojo
*/

#include "CreateEye.hpp"
#include "components/Hitbox.hpp"
#include "components/Position.hpp"
#include "entities/Sprite/CreateAnimateSprite.hpp"

namespace factories {

Entity createEye(Registry& registry,
    const std::string& textureOutlineId,
    const std::string& texturePupilId,
    float x, float y,
    int width, int height,
    int wS, int hS,
    int pupilW, int pupilH,
    float pupilRadius)
{
    Entity eye = registry.create_entity();
    registry.emplace<Position>(eye, x, y);

    Entity outline = createSprite(registry, textureOutlineId, x, y,
        width, height, wS, hS);
    Entity pupil = createSprite(registry, texturePupilId, x, y,
        width, height, wS, hS);

    // Attach Eye component to parent
    Eye eyeComp;
    eyeComp.outlineEntity = outline;
    eyeComp.pupilEntity = pupil;
    eyeComp.center_offset_x = 0;
    eyeComp.center_offset_y = 0;
    eyeComp.pupil_radius = pupilRadius;
    eyeComp.pupil_w = pupilW;
    eyeComp.pupil_h = pupilH;
    registry.add<Eye>(eye, eyeComp);

    return eye;
}

} // namespace factories
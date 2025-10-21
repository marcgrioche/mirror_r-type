/*
** BackgroundMenu.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 21 12:49:05 AM 2025 jojo
** Last update Wed Oct 21 3:06:11 PM 2025 jojo
*/

#pragma once
#include "../managers/GraphicsManager.hpp"
#include "Registry.hpp"
#include "entities/Sprite/CreateAnimateSprite.hpp"
#include "entities/eyes/CreateEye.hpp"
#include "render/EyeRender.hpp"
#include "render/SpriteRender.hpp"
#include <iostream>
#include <list>
#include <utility>
#include <vector>

class BackgroundMenu {
public:
    BackgroundMenu();
    ~BackgroundMenu();

    // draw takes gfx + registry so we can render and access entities
    void draw(GraphicsManager& gfx, Registry& registry);

    // destroy and reload operate on the registry
    void destroy(Registry& registry);

    // reload generates 'count' eyes and a background
    void reload(Registry& registry, int eyeCount, const std::vector<Entity>& existingEntities = {});

protected:
private:
    std::list<Entity> listEye;
    Entity m_background;

    std::vector<std::pair<std::string, std::string>> listEyeId;
};

/*
** HomeMenu.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 6:56:33 PM 2025 jojo
** Last update Sat Oct 10 3:56:45 PM 2025 jojo
*/

#pragma once
#include "Entity.hpp"
#include "Registry.hpp"
#include "managers/EventManager.hpp"
#include "managers/GraphicsManager.hpp"
#include "render/ButtonRender.hpp"
#include "render/TextBoxInputRender.hpp"
#include "systems/ButtonSystem.hpp"
#include "systems/TextBoxInputSystem.hpp"
#include <string>

class HomeMenu {
public:
    HomeMenu();
    ~HomeMenu();

    // Interface simple
    void show(Registry& registry);
    void hide(Registry& registry);
    bool isVisible() const { return m_visible; }

    // Update et render (à appeler si visible)
    void update(Registry& registry, float deltaTime);
    void render(GraphicsManager& gfx, Registry& registry);
    void handleEvent(Registry& registry, const SDL_Event& event);

    // Récupère le pseudo saisi
    std::string getPseudo(Registry& registry) const;

    // Vérifie les actions utilisateur
    bool hasJoinRequest() const;
    bool hasCreateRequest() const;
    bool hasParamRequest() const;
    void clearRequests();

private:
    Entity m_textBoxEntity;
    Entity m_paramButtonEntity;
    Entity m_joinButtonEntity;
    Entity m_createButtonEntity;
    Entity m_textBoxCreateEntity;
    Entity m_textBoxJoinEntity;
    Entity m_textBoxParamEntity;
    Entity m_ParamButtonEntity;
    bool m_visible = false;
    bool m_joinRequested = false;
    bool m_createRequested = false;
    bool m_paramRequested = false;

    void createEntities(Registry& registry);
    void destroyEntities(Registry& registry);
    void setupEventHandlers();
};

/*
** EndMenu.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 8 1:33:59 PM 2025 jojo
** Last update Thu Oct 8 2:32:55 PM 2025 jojo
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

class EndMenu {
public:
    EndMenu();
    ~EndMenu();

    // Interface simple
    void show(Registry& registry, bool State);
    void hide(Registry& registry);
    bool isVisible() const { return m_visible; }

    // Update et render (à appeler si visible)
    void update(Registry& registry, float deltaTime);
    void render(GraphicsManager& gfx, Registry& registry);
    void handleEvent(Registry& registry, const SDL_Event& event);

    // Vérifie si l'utilisateur a cliqué "Connect"
    bool hasReturnRequest() const;
    void clearConnectionRequest();

private:
    Entity m_textBoxStateEntity;
    Entity m_textBoxReturnEntity;
    Entity m_returnButtonEntity;
    bool m_visible = false;
    bool m_returnRequested = false;
    std::string m_state = "WIN";

    void createEntities(Registry& registry);
    void destroyEntities(Registry& registry);
    void setupEventHandlers();
};
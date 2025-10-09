/*
** JoinMenu.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 5:24:22 PM 2025 jojo
** Last update Thu Oct 8 2:33:02 PM 2025 jojo
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

class JoinMenu {
public:
    JoinMenu();
    ~JoinMenu();

    // Interface simple
    void show(Registry& registry);
    void hide(Registry& registry);
    bool isVisible() const { return m_visible; }

    // Update et render (à appeler si visible)
    void update(Registry& registry, float deltaTime);
    void render(GraphicsManager& gfx, Registry& registry);
    void handleEvent(Registry& registry, const SDL_Event& event);

    // Récupère le code de connexion saisi
    std::string getConnectionCode(Registry& registry) const;

    // Vérifie si l'utilisateur a cliqué "Connect"
    bool hasConnectionRequest() const;
    bool hasReturnRequest() const;
    void clearConnectionRequest();

private:
    Entity m_textBoxEntity;
    Entity m_connectButtonEntity;
    Entity m_textBoxConnectEntity;
    Entity m_returnButtonEntity;
    bool m_visible = false;
    bool m_connectionRequested = false;
    bool m_returnRequested = false;

    void createEntities(Registry& registry);
    void destroyEntities(Registry& registry);
    void setupEventHandlers();
};

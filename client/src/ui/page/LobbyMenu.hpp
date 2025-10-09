/*
** LobbyMenu.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 9:48:07 PM 2025 jojo
** Last update Thu Oct 8 2:33:05 PM 2025 jojo
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

class LobbyMenu {
public:
    LobbyMenu();
    ~LobbyMenu();

    // Interface simple
    void show(Registry& registry);
    void showAfterGameEnd(Registry& registry);
    void hide(Registry& registry);
    bool isVisible() const { return m_visible; }

    // Update et render (à appeler si visible)
    void update(Registry& registry, float deltaTime);
    void render(GraphicsManager& gfx, Registry& registry);
    void handleEvent(Registry& registry, const SDL_Event& event);

    // Vérifie si l'utilisateur a cliqué "Connect"
    bool hasRequest() const;
    bool hasReturnRequest() const;
    void clearRequests();

private:
    // Entity m_textBoxEntity;
    Entity m_connectButtonEntity;
    Entity m_connectTextBoxEntity;
    Entity m_returnButtonEntity;
    bool m_visible = false;
    bool m_LobbyRequested = false;
    bool m_returnRequested = false;
    bool m_isReturningFromGame = false;

    void createEntities(Registry& registry);
    void destroyEntities(Registry& registry);
    void setupEventHandlers();
};

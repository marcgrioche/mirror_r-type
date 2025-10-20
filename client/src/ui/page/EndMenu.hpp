/*
** EndMenuPage.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 8 1:33:59 PM 2025 jojo
** Last update Thu Oct 15 11:07:09 PM 2025 jojo
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

/**
 * @brief Class representing the end game menu page
 */
class EndMenuPage {
public:
    /**
     * @brief Constructs an EndMenuPage instance
     */
    EndMenuPage();

    /**
     * @brief Destroys the EndMenuPage instance and cleans up resources
     */
    ~EndMenuPage();

    // Interface simple

    /**
     * @brief Shows the end menu with the specified game state
     * @param registry The ECS registry to create entities in
     * @param State The game state (win/lose)
     */
    void show(Registry& registry, bool State);

    /**
     * @brief Hides the end menu by destroying its UI elements
     * @param registry The ECS registry containing the menu entities
     */
    void hide(Registry& registry);

    /**
     * @brief Checks if the end menu is currently visible
     * @return True if the menu is visible, false otherwise
     */
    bool isVisible() const { return m_visible; }

    // Update et render (à appeler si visible)

    /**
     * @brief Updates the end menu state
     * @param registry The ECS registry containing menu entities
     * @param deltaTime Time elapsed since last update
     */
    void update(Registry& registry, float deltaTime);

    /**
     * @brief Renders the end menu to the screen
     * @param gfx The graphics manager for rendering
     * @param registry The ECS registry containing menu entities
     */
    void render(GraphicsManager& gfx, Registry& registry);

    /**
     * @brief Handles SDL events for the end menu
     * @param registry The ECS registry containing menu entities
     * @param event The SDL event to process
     */
    void handleEvent(Registry& registry, const SDL_Event& event);

    // Vérifie si l'utilisateur a cliqué "Connect"

    /**
     * @brief Checks if the user has requested to return to main menu
     * @return True if return request is pending, false otherwise
     */
    bool hasReturnRequest() const;

    /**
     * @brief Clears all pending user requests
     */
    void clearConnectionRequest();

private:
    Entity m_textBoxStateEntity;
    Entity m_textBoxReturnEntity;
    Entity m_returnButtonEntity;
    Entity m_backgroundEntity;
    bool m_visible = false;
    bool m_returnRequested = false;
    std::string m_state = "WIN";

    void createEntities(Registry& registry);
    void destroyEntities(Registry& registry);
    void setupEventHandlers();
};

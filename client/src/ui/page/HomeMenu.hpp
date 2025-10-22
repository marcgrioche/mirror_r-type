/*
** HomeMenu.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 6:56:33 PM 2025 jojo
** Last update Wed Oct 21 2:59:25 PM 2025 jojo
*/

#pragma once
#include "../BackgroundMenu.hpp"
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
    /**
     * @brief Constructs a HomeMenu instance
     */
    HomeMenu();

    /**
     * @brief Destroys the HomeMenu instance and cleans up resources
     */
    ~HomeMenu();

    // Interface simple

    /**
     * @brief Shows the home menu by creating and displaying its UI elements
     * @param registry The ECS registry to create entities in
     */
    void show(Registry& registry);

    /**
     * @brief Hides the home menu by destroying its UI elements
     * @param registry The ECS registry containing the menu entities
     */
    void hide(Registry& registry);

    /**
     * @brief Checks if the home menu is currently visible
     * @return True if the menu is visible, false otherwise
     */
    bool isVisible() const { return m_visible; }

    // Update et render (à appeler si visible)

    /**
     * @brief Updates the home menu state
     * @param registry The ECS registry containing menu entities
     * @param deltaTime Time elapsed since last update
     */
    void update(Registry& registry, float deltaTime);

    /**
     * @brief Renders the home menu to the screen
     * @param gfx The graphics manager for rendering
     * @param registry The ECS registry containing menu entities
     */
    void render(GraphicsManager& gfx, Registry& registry);

    /**
     * @brief Handles SDL events for the home menu
     * @param registry The ECS registry containing menu entities
     * @param event The SDL event to process
     */
    void handleEvent(Registry& registry, const SDL_Event& event);

    // Récupère le pseudo saisi

    /**
     * @brief Gets the username entered in the text input field
     * @param registry The ECS registry containing the text input entity
     * @return The entered username as a string
     */
    std::string getPseudo(Registry& registry) const;

    // Vérifie les actions utilisateur

    /**
     * @brief Checks if the user has requested to join a lobby
     * @return True if join request is pending, false otherwise
     */
    bool hasJoinRequest() const;

    /**
     * @brief Checks if the user has requested to create a lobby
     * @return True if create request is pending, false otherwise
     */
    bool hasCreateRequest() const;

    /**
     * @brief Checks if the user has requested to open parameters/settings
     * @return True if parameter request is pending, false otherwise
     */
    bool hasParamRequest() const;

    /**
     * @brief Clears all pending user requests
     */
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
    BackgroundMenu m_backgroundMenu;

    bool m_visible = false;
    bool m_joinRequested = false;
    bool m_createRequested = false;
    bool m_paramRequested = false;

    void createEntities(Registry& registry);
    void destroyEntities(Registry& registry);
    void setupEventHandlers();
};

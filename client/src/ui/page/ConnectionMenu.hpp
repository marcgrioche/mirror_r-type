/*
** ConnectionMenu.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 5:24:22 PM 2025 jojo
** Last update Tue Oct 20 8:50:48 PM 2025 jojo
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

class ConnectionMenu {
public:
    /**
     * @brief Constructs a ConnectionMenu instance
     */
    ConnectionMenu();

    /**
     * @brief Destroys the ConnectionMenu instance and cleans up resources
     */
    ~ConnectionMenu();

    // Interface simple

    /**
     * @brief Shows the connection menu by creating and displaying its UI elements
     * @param registry The ECS registry to create entities in
     */
    void show(Registry& registry);

    /**
     * @brief Hides the connection menu by destroying its UI elements
     * @param registry The ECS registry containing the menu entities
     */
    void hide(Registry& registry);

    /**
     * @brief Checks if the connection menu is currently visible
     * @return True if the menu is visible, false otherwise
     */
    bool isVisible() const { return m_visible; }

    // Update et render (à appeler si visible)

    /**
     * @brief Updates the connection menu state
     * @param registry The ECS registry containing menu entities
     * @param deltaTime Time elapsed since last update
     */
    void update(Registry& registry, float deltaTime);

    /**
     * @brief Renders the connection menu to the screen
     * @param gfx The graphics manager for rendering
     * @param registry The ECS registry containing menu entities
     */
    void render(GraphicsManager& gfx, Registry& registry);

    /**
     * @brief Handles SDL events for the connection menu
     * @param registry The ECS registry containing menu entities
     * @param event The SDL event to process
     */
    void handleEvent(Registry& registry, const SDL_Event& event);

    // Récupère le code de connexion saisi

    /**
     * @brief Gets the connection code entered in the text input field
     * @param registry The ECS registry containing the text input entity
     * @return The entered connection code as a string
     */
    std::string getConnectionCode(Registry& registry) const;

    // Vérifie si l'utilisateur a cliqué "Connect"

    /**
     * @brief Checks if the user has requested to connect using the entered code
     * @return True if connection request is pending, false otherwise
     */
    bool hasConnectionRequest() const;

    /**
     * @brief Clears the pending connection request
     */
    void clearConnectionRequest();

private:
    Entity m_textBoxEntity;
    Entity m_connectButtonEntity;
    Entity m_connectTextBoxEntity;
    Entity m_backgroundEntity;
    Entity m_textBoxSpriteEntity;

    Entity m_eyetest;

    bool m_visible = false;
    bool m_connectionRequested = false;

    void createEntities(Registry& registry);
    void destroyEntities(Registry& registry);
    void setupEventHandlers();
};

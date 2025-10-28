/*
** LobbyMenu.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 9:48:07 PM 2025 jojo
** Last update Thu Oct 15 11:07:35 PM 2025 jojo
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
#include <unordered_map>

class LobbyMenu {
public:
    /**
     * @brief Constructs a LobbyMenu instance
     */
    LobbyMenu();

    /**
     * @brief Destroys the LobbyMenu instance and cleans up resources
     */
    ~LobbyMenu();

    // Interface simple

    /**
     * @brief Shows the lobby menu by creating and displaying its UI elements
     * @param registry The ECS registry to create entities in
     * @param lobbyId The lobby ID to display
     * @param currentLevel The player's current level
     * @param maxLevel The maximum available level
     */
    void show(Registry& registry, int lobbyId, uint32_t currentLevel, uint32_t maxLevel);

    /**
     * @brief Shows the lobby menu after a game has ended
     * @param registry The ECS registry to create entities in
     * @param lobbyId The lobby ID to display
     * @param currentLevel The player's current level
     * @param maxLevel The maximum available level
     */
    void showAfterGameEnd(Registry& registry, uint32_t lobbyId, uint32_t currentLevel, uint32_t maxLevel);

    /**
     * @brief Hides the lobby menu by destroying its UI elements
     * @param registry The ECS registry containing the menu entities
     */
    void hide(Registry& registry);

    /**
     * @brief Checks if the lobby menu is currently visible
     * @return True if the menu is visible, false otherwise
     */
    bool isVisible() const { return m_visible; }

    // Update et render (à appeler si visible)

    /**
     * @brief Updates the lobby menu state
     * @param registry The ECS registry containing menu entities
     * @param deltaTime Time elapsed since last update
     */
    void update(Registry& registry, float deltaTime);

    /**
     * @brief Renders the lobby menu to the screen
     * @param gfx The graphics manager for rendering
     * @param registry The ECS registry containing menu entities
     */
    void render(GraphicsManager& gfx, Registry& registry);

    /**
     * @brief Handles SDL events for the lobby menu
     * @param registry The ECS registry containing menu entities
     * @param event The SDL event to process
     */
    void handleEvent(Registry& registry, const SDL_Event& event);

    /**
     * @brief Renders the dungeon map showing level progression
     * @param renderer The SDL renderer to use for drawing
     */
    void renderDungeonMap(SDL_Renderer* renderer);

    // Vérifie si l'utilisateur a cliqué "Connect"

    /**
     * @brief Checks if the user has requested to connect to a lobby
     * @return True if connect request is pending, false otherwise
     */
    bool hasRequest() const;

    /**
     * @brief Checks if the user has requested to return to main menu
     * @return True if return request is pending, false otherwise
     */
    bool hasReturnRequest() const;

    /**
     * @brief Clears all pending user requests
     */
    void clearRequests();

    /**
     * @brief Sets the player names for display in the lobby
     * @param players Map of player IDs to usernames
     */
    void setPlayerNames(const std::unordered_map<uint32_t, std::string>& players);

    /**
     * @brief Updates the player entities in the lobby menu
     * @param registry The ECS registry to update entities in
     */
    void updatePlayerEntities(Registry& registry);

private:
    // Entity m_textBoxEntity;
    Entity m_connectButtonEntity;
    Entity m_connectTextBoxEntity;
    Entity m_returnButtonEntity;
    Entity m_textBoxLobbyEntity;
    Entity m_backgroundEntity;
    std::vector<Entity> m_playerTextEntities;
    std::unordered_map<uint32_t, std::string> m_playerNames;
    bool m_visible = false;
    bool m_LobbyRequested = false;
    bool m_returnRequested = false;
    bool m_isReturningFromGame = false;
    uint32_t m_currentLevel = 1;
    uint32_t m_maxLevel = 1;

    void createEntities(Registry& registry, int lobbyId);
    void destroyEntities(Registry& registry);
    void setupEventHandlers();
};

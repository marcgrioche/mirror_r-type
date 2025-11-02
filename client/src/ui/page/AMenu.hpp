#pragma once
#include "Registry.hpp"
#include "ui/BackgroundMenu.hpp"

class AMenu {
public:
    enum class Input {
        USERNAME,
        PASSWORD,
    };
    AMenu() = default;
    virtual ~AMenu() = default;

    /**
     * @brief Shows the menu by creating and displaying its UI elements
     * @param registry The ECS registry to create entities in
     */
    virtual void show(Registry& registry);

    /**
     * @brief Hides the menu by destroying its UI elements
     * @param registry The ECS registry containing the menu entities
     */
    void hide(Registry& registry);

    /**
     * @brief Updates the menu state
     * @param registry The ECS registry containing menu entities
     * @param deltaTime Time elapsed since last update
     */
    void update(Registry& registry, float deltaTime);

    /**
     * @brief Creates the menu entities in the ECS registry
     * @param registry The ECS registry to create entities in
     */
    virtual void createEntities(Registry& registry) = 0;

    /**
     * @brief Destroys the menu entities from the ECS registry
     * @param registry The ECS registry containing the menu entities
     */
    void destroyEntities(Registry& registry);

    /**
     * @brief Handles SDL events for the menu
     * @param registry The ECS registry containing menu entities
     * @param event The SDL event to handle
     */
    virtual void handleEvent(Registry& registry, const SDL_Event& event);

    /**
     * @brief Renders the menu to the screen
     * @param gfx The graphics manager for rendering
     * @param registry The ECS registry containing menu entities
     */
    virtual void render(GraphicsManager& gfx, Registry& registry) = 0;

    /**
     * @brief Sets up event handlers for the menu
     */
    virtual void setupEventHandlers() = 0;

    /**
     * @brief Resets other menu-specific options when showing the menu
     */
    virtual void resetOtherOptions() = 0;

    /**
     * @brief Clears all pending user requests
     */
    virtual void clearRequests() = 0;

    /**
     * @brief Checks if the user has made any requests
     * @return True if any request is pending, false otherwise
     */
    virtual bool hasRequest() const = 0;

    /**
     * @brief Retrieves input data from the menu
     * @param registry The ECS registry containing menu entities
     * @param input The type of input data to retrieve
     * @return The requested input data as a string
     */
    virtual std::string getInputData(Registry& registry, Input input) = 0;

    /**
     * @brief Checks if the menu is currently visible
     * @return True if the menu is visible, false otherwise
     */
    bool isVisible() const;

protected:
    bool m_visible = false;
    BackgroundMenu m_bg;
    std::unordered_map<std::string, Entity> m_entities;
};

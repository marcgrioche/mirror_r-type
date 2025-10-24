#pragma once
#include "Entity.hpp"
#include "Registry.hpp"
#include "managers/GraphicsManager.hpp"
#include "page/ConnectionMenu.hpp"
#include "page/EndMenu.hpp"
#include "page/HomeMenu.hpp"
#include "page/JoinMenu.hpp"
#include "page/LobbyMenu.hpp"
#include "page/ParameterMenu.hpp"
#include <SDL.h>

#include "page/LoginMenu.hpp"

class Menu {
public:
    /**
     * @brief Enumeration of available menu pages
     */
    enum class Page {
        HOME, /**< Main home page */
        CONNECTION, /**< Server connection page */
        LOGIN, /**< Server login / register page */
        JOIN_LOBBY, /**< Lobby joining page */
        LOBBY, /**< Lobby waiting page */
        PARAMETERS, /**< Settings/parameters page */
        WIN, /**< Game victory page */
        LOSE /**< Game defeat page */
    };

    /**
     * @brief Default constructor for Menu
     */
    Menu();

    /**
     * @brief Destructor for Menu
     */
    ~Menu();

    // Interface principale

    /**
     * @brief Activates the menu system and shows the specified page
     * @param registry The ECS registry to use for entity management
     * @param page The initial page to display (default: HOME)
     */
    void activate(Registry& registry, Page page = Page::HOME);

    /**
     * @brief Deactivates the menu system and hides all pages
     * @param registry The ECS registry to use for entity management
     */
    void deactivate(Registry& registry);

    /**
     * @brief Checks if the menu system is currently active
     * @return True if the menu is active, false otherwise
     */
    bool isActive() const { return m_active; }

    // Gestion des événements et rendu

    /**
     * @brief Handles SDL events for the menu system
     * @param e The SDL event to process
     * @param registry The ECS registry to use for entity management
     */
    void handleEvent(const SDL_Event& e, Registry& registry);

    /**
     * @brief Updates the menu system state
     * @param registry The ECS registry to use for entity management
     * @param deltaTime Time elapsed since last update in seconds
     */
    void update(Registry& registry, float deltaTime);

    /**
     * @brief Renders the current menu page
     * @param gfx The graphics manager for rendering
     * @param registry The ECS registry to use for entity management
     */
    void render(GraphicsManager& gfx, Registry& registry);

    // Navigation entre pages

    /**
     * @brief Shows the home page
     * @param registry The ECS registry to use for entity management
     */
    void showHomePage(Registry& registry);

    /**
     * @brief Shows the connection page
     * @param registry The ECS registry to use for entity management
     */
    void showConnectionPage(Registry& registry);

    void showLoginPage(Registry& registry);
    /**
     * @brief Shows the lobby joining page
     * @param registry The ECS registry to use for entity management
     */
    void showJoinPage(Registry& registry);

    /**
     * @brief Shows the parameters/settings page
     * @param registry The ECS registry to use for entity management
     */
    void showParametersPage(Registry& registry);

    /**
     * @brief Shows the victory page
     * @param registry The ECS registry to use for entity management
     */
    void showWinPage(Registry& registry);

    /**
     * @brief Shows the defeat page
     * @param registry The ECS registry to use for entity management
     */
    void showLosePage(Registry& registry);

    /**
     * @brief Shows the lobby page
     * @param registry The ECS registry to use for entity management
     */
    void showLobbyPage(Registry& registry, int lobbyId);

    /**
     * @brief Shows the lobby page after a game has ended
     * @param registry The ECS registry to use for entity management
     */
    void showLobbyPageAfterGame(Registry& registry);

    // Récupération des données saisies

    /**
     * @brief Gets the connection code entered by the user
     * @param registry The ECS registry to use for entity management
     * @return The connection code string
     */
    std::string getConnectionCode(Registry& registry) const;

    /**
     * @brief Gets the lobby join code entered by the user
     * @param registry The ECS registry to use for entity management
     * @return The join code string
     */
    std::string getJoinCode(Registry& registry) const;

    std::string getInput(Registry& registry, AMenu::Input inputType);

    /**
     * @brief Gets the username entered by the user
     * @param registry The ECS registry to use for entity management
     * @return The username string
     */
    std::string getUserPseudo(Registry& registry) const;

    // Vérification des demandes utilisateur

    /**
     * @brief Checks if the user has requested to connect to a server
     * @return True if connection request is pending, false otherwise
     */
    bool hasConnectionRequest() const;

    bool hasLoginRequest() const;

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
     * @brief Checks if the user has requested to enter the lobby
     * @return True if lobby request is pending, false otherwise
     */
    bool hasLobbyRequest() const;

    /**
     * @brief Checks if the user has requested to end the current action
     * @return True if end request is pending, false otherwise
     */
    bool hasEndRequest() const;

    /**
     * @brief Checks if the user has requested to change parameters/settings
     * @return True if parameter change request is pending, false otherwise
     */
    bool hasParameterChanges() const;

    // Nettoyage des flags de demande

    /**
     * @brief Clears all pending user request flags
     */
    void clearAllRequests();

    // Entity management

    /**
     * @brief Clears all game-related entities from the registry
     * @param registry The ECS registry to clear entities from
     */
    void clearGameEntities(Registry& registry);

private:
    bool m_active = false;
    Page m_currentPage = Page::HOME;

    // Pages modulaires
    HomeMenu m_homePage;
    ConnectionMenu m_connectionPage;
    JoinMenu m_joinPage;
    LobbyMenu m_lobbyPage;
    EndMenuPage m_endPage;
    ParameterMenu m_parameterPage;
    LoginMenu m_loginPage;

    // Gestion des transitions
    void hideAllPages(Registry& registry);
    void processPageTransitions(Registry& registry);
};

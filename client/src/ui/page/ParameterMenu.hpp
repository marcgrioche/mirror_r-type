/*
** ParameterMenu.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 8 12:54:23 PM 2025 jojo
** Last update Sat Oct 10 4:35:30 PM 2025 jojo
*/

#pragma once
#include "Registry.hpp"
#include "entities/button/CreateButton.hpp"
#include "entities/textbox/TextBox.hpp"
#include "managers/EventManager.hpp"
#include "managers/GraphicsManager.hpp"
#include "managers/InputManager.hpp"
#include "render/ButtonRender.hpp"
#include "render/TextBoxInputRender.hpp"
#include "render/TextBoxRender.hpp"
#include "systems/ButtonSystem.hpp"
#include "systems/TextBoxInputSystem.hpp"
#include <SDL.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class ParameterMenu {
public:
    ParameterMenu();
    ~ParameterMenu();

    void show(Registry& registry);
    void hide(Registry& registry);
    void update(Registry& registry, float dt);
    void render(GraphicsManager& gfx, Registry& registry);
    void handleEvent(Registry& registry, const SDL_Event& event);

    bool isVisible() const { return m_visible; }
    bool hasReturnRequest() const { return m_returnRequested; }
    void clearRequests() { m_returnRequested = false; }

private:
    void createEntities(Registry& registry);
    void destroyEntities(Registry& registry);
    void setupEventHandlers();

    void rebuildTextEntities(Registry& registry);

    // existing persistence helpers
    void loadBindings();
    void saveBindings();
    std::string keysToString(const std::vector<int>& keys) const;

private:
    bool m_visible = false;
    bool m_waitingForKey = false;
    bool m_returnRequested = false;
    size_t m_selectedIndex = 0;
    std::string m_iniPath = "client/res/config/keybindings.ini";

    // pair: action name -> list of keycodes (persisted)
    std::vector<std::pair<std::string, std::vector<int>>> m_bindings;

    std::vector<Entity> m_actionTextEntities; // left column
    std::vector<Entity> m_keyTextEntities; // right column
    Entity m_returnButtonEntity { 0, 0 };
    Entity m_returnTextEntity { 0, 0 };
};

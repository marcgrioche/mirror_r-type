/*
** ParameterMenu.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Fri Oct 10 3:47:00 PM 2025 jojo
** Last update Sat Oct 10 4:38:33 PM 2025 jojo
*/

#include "ParameterMenu.hpp"
#include "entities/button/CreateButton.hpp"
#include "entities/textbox/TextBox.hpp"
#include <iostream>

ParameterMenu::ParameterMenu()
{
    loadBindings();
    setupEventHandlers();
}

ParameterMenu::~ParameterMenu()
{
    // nothing
}

void ParameterMenu::show(Registry& registry)
{
    if (!m_visible) {
        createEntities(registry);
        m_visible = true;
        m_waitingForKey = false;
        m_returnRequested = false;
        if (m_selectedIndex >= m_bindings.size())
            m_selectedIndex = 0;
    }
}

void ParameterMenu::hide(Registry& registry)
{
    if (m_visible) {
        destroyEntities(registry);
        m_visible = false;
        m_waitingForKey = false;
    }
}

void ParameterMenu::createEntities(Registry& registry)
{
    // destroy first if somehow present
    destroyEntities(registry);

    // Build text entities for each binding
    rebuildTextEntities(registry);

    // return button
    m_returnButtonEntity = factories::createButton(registry, 320.0f, 520.0f, 160.0f, 50.0f, "return_to_home", true);
    m_returnTextEntity = factories::createTextBox(registry, "RETURN", 360.0f, 535.0f, 16, Color { 255, 255, 255, 255 }, ::TextBox::Alignment::CENTER);
}

void ParameterMenu::destroyEntities(Registry& registry)
{
    for (Entity e : m_actionTextEntities) {
        if (registry.has<TextBox>(e) || registry.has<Position>(e))
            registry.kill_entity(e);
    }
    m_actionTextEntities.clear();

    for (Entity e : m_keyTextEntities) {
        if (registry.has<TextBox>(e) || registry.has<Position>(e))
            registry.kill_entity(e);
    }
    m_keyTextEntities.clear();

    if (m_returnTextEntity.id != 0) {
        registry.kill_entity(m_returnTextEntity);
        m_returnTextEntity = { 0, 0 };
    }
    if (m_returnButtonEntity.id != 0) {
        registry.kill_entity(m_returnButtonEntity);
        m_returnButtonEntity = { 0, 0 };
    }
}

void ParameterMenu::setupEventHandlers()
{
    auto& eventMgr = EventManager::getInstance();
    eventMgr.subscribe(EventType::BUTTON_CLICK, [this](const GameEvent& event) {
        if (event.data == "return_to_home" && m_visible) {
            m_returnRequested = true;
        }
    });
}

void ParameterMenu::update(Registry& registry, float deltaTime)
{
    if (!m_visible)
        return;

    // update systems visuals (cursor blink...)
    textBoxInputUpdateSystem(registry, deltaTime);
    buttonSystem(registry);

    // Ensure key display boxes reflect current bindings
    for (size_t i = 0; i < m_keyTextEntities.size() && i < m_bindings.size(); ++i) {
        if (registry.has<TextBox>(m_keyTextEntities[i])) {
            registry.get<TextBox>(m_keyTextEntities[i]).text = keysToString(m_bindings[i].second);
            // highlight selected by changing color
            if (i == m_selectedIndex) {
                registry.get<TextBox>(m_keyTextEntities[i]).color = Color { 255, 200, 80, 255 };
            } else {
                registry.get<TextBox>(m_keyTextEntities[i]).color = Color { 200, 200, 200, 255 };
            }
        }
    }
}

void ParameterMenu::handleEvent(Registry& registry, const SDL_Event& event)
{
    if (!m_visible)
        return;

    // delegate to textbox input system for paste/etc (not used for page, but safe)
    textBoxInputSystem(registry, event);

    // existing parameter handling
    if (event.type == SDL_KEYDOWN) {
        SDL_Keycode kc = event.key.keysym.sym;

        if (!m_waitingForKey && (kc == SDLK_ESCAPE || kc == SDLK_BACKSPACE)) {
            m_returnRequested = true;
            return;
        }

        if (m_waitingForKey) {
            int keycode = static_cast<int>(kc);
            if (m_selectedIndex < m_bindings.size()) {
                m_bindings[m_selectedIndex].second.clear();
                m_bindings[m_selectedIndex].second.push_back(keycode);
                saveBindings();
                std::cout << "Rebound " << m_bindings[m_selectedIndex].first
                          << " to " << SDL_GetKeyName(kc) << " (" << keycode << ")\n";
            }
            m_waitingForKey = false;
            return;
        }

        if (kc == SDLK_w || kc == SDLK_UP) {
            if (m_selectedIndex > 0)
                m_selectedIndex--;
        } else if (kc == SDLK_s || kc == SDLK_DOWN) {
            if (m_selectedIndex + 1 < m_bindings.size())
                m_selectedIndex++;
        } else if (kc == SDLK_RETURN || kc == SDLK_KP_ENTER) {
            m_waitingForKey = true;
            std::cout << "Press a key to bind to action: " << m_bindings[m_selectedIndex].first << std::endl;
        }
    }

    // mouse selection: map Y position to index (similar to earlier approach)
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mx = event.button.x;
        int my = event.button.y;
        int top = 120;
        int lineH = 36;
        if (my >= top) {
            size_t idx = (my - top) / lineH;
            if (idx < m_bindings.size()) {
                m_selectedIndex = idx;
                m_waitingForKey = true;
                std::cout << "Click -> rebinding " << m_bindings[m_selectedIndex].first << std::endl;
            }
        }
    }
}

void ParameterMenu::rebuildTextEntities(Registry& registry)
{
    // destroy any previous ones
    for (Entity e : m_actionTextEntities)
        if (registry.has<TextBox>(e) || registry.has<Position>(e))
            registry.kill_entity(e);
    for (Entity e : m_keyTextEntities)
        if (registry.has<TextBox>(e) || registry.has<Position>(e))
            registry.kill_entity(e);
    m_actionTextEntities.clear();
    m_keyTextEntities.clear();

    // layout constants
    float leftX = 120.0f;
    float rightX = 420.0f;
    int top = 120;
    int lineH = 36;

    for (size_t i = 0; i < m_bindings.size(); ++i) {
        float y = static_cast<float>(top + static_cast<int>(i) * lineH);

        Entity a = factories::createTextBox(registry, m_bindings[i].first, leftX, y, 16, Color { 200, 200, 200, 255 }, ::TextBox::Alignment::LEFT);
        Entity k = factories::createTextBox(registry, keysToString(m_bindings[i].second), rightX, y, 16, Color { 200, 200, 200, 255 }, ::TextBox::Alignment::LEFT);

        m_actionTextEntities.push_back(a);
        m_keyTextEntities.push_back(k);
    }
}

void ParameterMenu::render(GraphicsManager& gfx, Registry& registry)
{
    if (!m_visible)
        return;

    SDL_Renderer* renderer = gfx.getRenderer();
    if (!renderer)
        return;

    // title
    Entity title = registry.create_entity();
    registry.emplace<Position>(title, 300.0f, 60.0f);
    registry.emplace<TextBox>(title, "KEY BINDINGS", 24, Color { 255, 255, 255, 255 }, "client/res/fonts/OpenSans-Medium.ttf", true, ::TextBox::Alignment::CENTER);
    drawTextBox(gfx, registry, title);
    registry.kill_entity(title);

    // draw entries
    for (Entity e : m_actionTextEntities)
        drawTextBox(gfx, registry, e);
    for (Entity e : m_keyTextEntities)
        drawTextBox(gfx, registry, e);

    // draw return button + text
    if (m_returnButtonEntity.id != 0)
        drawButton(gfx, registry, m_returnButtonEntity);
    if (m_returnTextEntity.id != 0)
        drawTextBox(gfx, registry, m_returnTextEntity);
}

void ParameterMenu::loadBindings()
{
    m_bindings.clear();
    std::ifstream ifs(m_iniPath);
    if (!ifs.is_open()) {
        std::cerr << "ParameterMenu: can't open " << m_iniPath << ", using defaults\n";
        // fallback defaults
        m_bindings = {
            { "move_up", { 122, 273 } },
            { "move_down", { 115, 274 } },
            { "move_left", { 113, 276 } },
            { "move_right", { 100, 275 } },
            { "shoot", { 32 } },
            { "quit", { 27 } }
        };
        return;
    }

    std::string line;
    while (std::getline(ifs, line)) {
        // trim
        if (line.empty())
            continue;
        if (line[0] == '#')
            continue;
        auto pos = line.find('=');
        if (pos == std::string::npos)
            continue;
        std::string key = line.substr(0, pos);
        std::string vals = line.substr(pos + 1);
        // parse csv of ints
        std::vector<int> codes;
        std::istringstream ss(vals);
        std::string token;
        while (std::getline(ss, token, ',')) {
            try {
                int v = std::stoi(token);
                codes.push_back(v);
            } catch (...) { /* ignore */
            }
        }
        if (!key.empty())
            m_bindings.emplace_back(key, codes);
    }
}

void ParameterMenu::saveBindings()
{
    std::ofstream ofs(m_iniPath, std::ios::trunc);
    if (!ofs.is_open()) {
        std::cerr << "ParameterMenu: can't write " << m_iniPath << std::endl;
        return;
    }
    ofs << "# R-Type Keybindings Configuration\n";
    ofs << "# Format: action=keycode1,keycode2,...\n\n";
    for (auto& entry : m_bindings) {
        ofs << entry.first << "=";
        for (size_t i = 0; i < entry.second.size(); ++i) {
            if (i)
                ofs << ",";
            ofs << entry.second[i];
        }
        ofs << "\n";
    }
    ofs.close();
    std::cout << "ParameterMenu: saved keybindings to " << m_iniPath << std::endl;

    try {
        InputManager::getInstance().reloadKeyBindings();
        std::cout << "ParameterMenu: reloaded InputManager keybindings" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "ParameterMenu: failed to reload InputManager: " << e.what() << std::endl;
    }
}

std::string ParameterMenu::keysToString(const std::vector<int>& keys) const
{
    if (keys.empty())
        return "UNBOUND";
    std::ostringstream ss;
    for (size_t i = 0; i < keys.size(); ++i) {
        if (i)
            ss << ",";
        ss << SDL_GetKeyName(static_cast<SDL_Keycode>(keys[i]));
    }
    return ss.str();
}

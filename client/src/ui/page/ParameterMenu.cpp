/*
** ParameterMenu.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ui/page
**
** Made by jojo
** Login   <jojo>
**
** Started on  Fri Oct 10 3:47:00 PM 2025 jojo
** Last update Tue Oct 13 3:24:36 PM 2025 jojo
*/

#include "ParameterMenu.hpp"
#include "entities/button/CreateButton.hpp"
#include "entities/textbox/TextBox.hpp"
#include "managers/ConfigManager.hpp"
#include <iostream>

ParameterMenu::ParameterMenu()
{
    loadBindings();
    m_colorblindType = ConfigManager::getInstance().getColorblindType();
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

    // auto-shoot toggle button
    m_autoShootButtonEntity = factories::createButton(registry, 120.0f, 450.0f, 100.0f, 40.0f, "auto_shoot_toggle", true);
    m_autoShootTextEntity = factories::createTextBox(registry, m_autoShoot ? "AUTO ON" : "AUTO OFF", 170.0f, 465.0f, 16, Color { 255, 255, 255, 255 }, ::TextBox::Alignment::LEFT);

    // colorblind toggle button
    m_colorblindButtonEntity = factories::createButton(registry, 120.0f, 500.0f, 100.0f, 40.0f, "colorblind_toggle", true);
    std::string colorblindDisplay = m_colorblindType.empty() ? "NONE" : m_colorblindType;
    m_colorblindTextEntity = factories::createTextBox(registry, colorblindDisplay, 170.0f, 515.0f, 16, Color { 255, 255, 255, 255 }, ::TextBox::Alignment::LEFT);

    // return button
    m_returnButtonEntity = factories::createButton(registry, 320.0f, 570.0f, 160.0f, 50.0f, "return_to_home", true);
    m_returnTextEntity = factories::createTextBox(registry, "RETURN", 360.0f, 585.0f, 16, Color { 255, 255, 255, 255 }, ::TextBox::Alignment::LEFT);
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

    registry.kill_entity(m_returnTextEntity);
    registry.kill_entity(m_returnButtonEntity);

    registry.kill_entity(m_autoShootTextEntity);
    registry.kill_entity(m_autoShootButtonEntity);

    registry.kill_entity(m_colorblindTextEntity);
    registry.kill_entity(m_colorblindButtonEntity);
}

void ParameterMenu::setupEventHandlers()
{
    auto& eventMgr = EventManager::getInstance();
    eventMgr.subscribe(EventType::BUTTON_CLICK, [this](const GameEvent& event) {
        if (event.data == "return_to_home" && m_visible) {
            m_returnRequested = true;
        } else if (event.data == "auto_shoot_toggle" && m_visible) {
            m_autoShoot = !m_autoShoot;
            saveBindings();
        } else if (event.data == "colorblind_toggle" && m_visible) {
            // Cycle: none -> protanopia -> deuteranopia -> tritanopia -> none
            if (m_colorblindType.empty()) {
                m_colorblindType = "protanopia";
            } else if (m_colorblindType == "protanopia") {
                m_colorblindType = "deuteranopia";
            } else if (m_colorblindType == "deuteranopia") {
                m_colorblindType = "tritanopia";
            } else {
                m_colorblindType = "";
            }
            ConfigManager::getInstance().setColorblindType(m_colorblindType);
            saveBindings();
            GraphicsManager::getInstance().setColorblindMode(m_colorblindType);
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

    if (registry.has<TextBox>(m_autoShootTextEntity)) {
        registry.get<TextBox>(m_autoShootTextEntity).text = m_autoShoot ? "AUTO ON" : "AUTO OFF";
    }

    if (registry.has<TextBox>(m_colorblindTextEntity)) {
        std::string colorblindDisplay = m_colorblindType.empty() ? "NONE" : m_colorblindType;
        registry.get<TextBox>(m_colorblindTextEntity).text = colorblindDisplay;
    }
}

void ParameterMenu::handleEvent(Registry& registry, const SDL_Event& event)
{
    if (!m_visible)
        return;

    // delegate to textbox input system for paste/etc (not used for page, but safe)
    textBoxInputSystem(registry, event);

    // existing parameter handling
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (m_waitingForKey) {
            int mouseBtn = static_cast<int>(event.button.button);
            int storedCode = -mouseBtn; // negative => mouse button
            if (m_selectedIndex < m_bindings.size()) {
                m_bindings[m_selectedIndex].second.clear();
                m_bindings[m_selectedIndex].second.push_back(storedCode);
                saveBindings();
                std::string btnName = (mouseBtn == SDL_BUTTON_LEFT) ? "Mouse Left" : (mouseBtn == SDL_BUTTON_RIGHT) ? "Mouse Right"
                    : (mouseBtn == SDL_BUTTON_MIDDLE)                                                               ? "Mouse Middle"
                                                                                                                    : "Mouse Button " + std::to_string(mouseBtn);
                std::cout << "Rebound " << m_bindings[m_selectedIndex].first
                          << " to " << btnName << " (" << storedCode << ")\n";
            }
            m_waitingForKey = false;
            return;
        }

        // not waiting: use click to select/rebind (existing behavior)
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
        return;
    }
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
            }
            m_waitingForKey = false;
            return;
        }

        if (kc == SDLK_z || kc == SDLK_UP) {
            if (m_selectedIndex > 0)
                m_selectedIndex--;
        } else if (kc == SDLK_s || kc == SDLK_DOWN) {
            if (m_selectedIndex + 1 < m_bindings.size())
                m_selectedIndex++;
        } else if (kc == SDLK_RETURN || kc == SDLK_KP_ENTER) {
            m_waitingForKey = true;
        }
    }

    // mouse selection: map Y position to index (similar to earlier approach)
    // TODO make a function
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mx = event.button.x;
        (void)mx;
        int my = event.button.y;
        int top = 120;
        int lineH = 36;
        if (my >= top) {
            size_t idx = (my - top) / lineH;
            if (idx < m_bindings.size()) {
                m_selectedIndex = idx;
                m_waitingForKey = true;
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

    // draw entries
    for (Entity e : m_actionTextEntities)
        drawTextBox(gfx, registry, e);
    for (Entity e : m_keyTextEntities)
        drawTextBox(gfx, registry, e);

    if (m_autoShootButtonEntity.id != 0)
        drawButton(gfx, registry, m_autoShootButtonEntity);
    if (m_autoShootTextEntity.id != 0)
        drawTextBox(gfx, registry, m_autoShootTextEntity);

    if (m_colorblindButtonEntity.id != 0)
        drawButton(gfx, registry, m_colorblindButtonEntity);
    if (m_colorblindTextEntity.id != 0)
        drawTextBox(gfx, registry, m_colorblindTextEntity);

    // draw return button + text
    if (m_returnButtonEntity.id != 0)
        drawButton(gfx, registry, m_returnButtonEntity);
    if (m_returnTextEntity.id != 0)
        drawTextBox(gfx, registry, m_returnTextEntity);
}

void ParameterMenu::loadBindings()
{
    m_bindings.clear();
    std::ifstream ifs(ConfigManager::getInstance().getConfigFilePath());
    if (!ifs.is_open()) {
        std::cerr << "ParameterMenu: can't open " << ConfigManager::getInstance().getConfigFilePath() << ", using defaults\n";
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
        key.erase(key.find_last_not_of(" \t") + 1);
        key.erase(0, key.find_first_not_of(" \t"));
        vals.erase(vals.find_last_not_of(" \t") + 1);
        vals.erase(0, vals.find_first_not_of(" \t"));
        if (key == "auto_shoot") {
            try {
                m_autoShoot = (std::stoi(vals) != 0);
            } catch (...) {
                m_autoShoot = false;
            }
        } else if (key == "colorblind_type") {
            if (vals == "protanopia" || vals == "deuteranopia" || vals == "tritanopia" || vals.empty()) {
                m_colorblindType = vals;
            } else {
                m_colorblindType = "";
            }
        } else if (!key.empty()) {
            // parse csv of ints
            std::vector<int> codes;
            std::istringstream ss(vals);
            std::string token;
            while (std::getline(ss, token, ',')) {
                try {
                    int v = std::stoi(token);
                    codes.push_back(v);
                } catch (...) {
                }
            }
            m_bindings.emplace_back(key, codes);
        }
    }
}

void ParameterMenu::saveBindings()
{
    std::ofstream ofs(ConfigManager::getInstance().getConfigFilePath(), std::ios::trunc);
    if (!ofs.is_open()) {
        std::cerr << "ParameterMenu: can't write " << ConfigManager::getInstance().getConfigFilePath() << std::endl;
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
    ofs << "auto_shoot=" << (m_autoShoot ? "1" : "0") << "\n";
    ofs << "colorblind_type=" << m_colorblindType << "\n";
    ofs.close();
    std::cout << "ParameterMenu: saved keybindings to " << ConfigManager::getInstance().getConfigFilePath() << std::endl;

    try {
        ConfigManager::getInstance().loadKeyBindings();
        InputManager::getInstance().reloadKeyBindings();
        std::cout << "ParameterMenu: reloaded keybindings" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "ParameterMenu: failed to reload keybindings: " << e.what() << std::endl;
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
        int code = keys[i];
        if (code < 0) { // mouse button encoding (negative)
            int mb = -code;
            std::string name = (mb == SDL_BUTTON_LEFT) ? "Mouse Left" : (mb == SDL_BUTTON_RIGHT) ? "Mouse Right"
                : (mb == SDL_BUTTON_MIDDLE)                                                      ? "Mouse Middle"
                                                                                                 : "Mouse " + std::to_string(mb);
            ss << name;
        } else {
            ss << SDL_GetKeyName(static_cast<SDL_Keycode>(code));
        }
    }
    return ss.str();
}

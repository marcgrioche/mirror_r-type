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

    virtual void show(Registry& registry);
    void hide(Registry& registry);
    void update(Registry& registry, float deltaTime);
    virtual void createEntities(Registry& registry) = 0;
    void destroyEntities(Registry& registry);
    virtual void handleEvent(Registry& registry, const SDL_Event& event);
    virtual void render(GraphicsManager& gfx, Registry& registry) = 0;
    virtual void setupEventHandlers() = 0;
    virtual void resetOtherOptions() = 0;
    virtual void clearRequests() = 0;
    virtual bool hasRequest() const = 0;
    virtual std::string getInputData(Registry& registry, Input input) = 0;
    bool isVisible() const;

protected:
    bool m_visible = false;
    BackgroundMenu m_bg;
    std::unordered_map<std::string, Entity> m_entities;
};

#pragma once
#include "AMenu.hpp"
#include "Registry.hpp"
#include "ui/BackgroundMenu.hpp"

class LoginMenu : public AMenu {
public:
    LoginMenu();

    void render(GraphicsManager& gfx, Registry& registry) override;
    void resetOtherOptions() override { m_loginRequested = false; };
    void clearRequests() override;
    bool hasRequest() const override;
    std::string getInputText(Registry& registry, const std::string& inputRef);
    std::string getInputData(Registry& registry, Input input) override;

private:
    // Entity m_usernameTextBoxEntity;
    // Entity m_passwordTextBoxEntity;
    // Entity m_loginButtonEntity;

    bool m_loginRequested = false;

    void createEntities(Registry& registry) override;
    void setupEventHandlers() override;
};

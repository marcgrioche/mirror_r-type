#include "Menu.hpp"
#include "../entities/button/CreateButton.hpp"
#include "../managers/GraphicsManager.hpp"
#include "components/AllComponents.hpp"

#include <filesystem>
#include <iostream>

void Menu::activate(Page page)
{
    m_active = true;
    m_page = page;
    m_inputFocused = (page == Page::Connect || page == Page::Join);
    m_requestConnect = m_requestCreate = m_requestJoin = m_requestStart = false;

    m_joinButtonCreated = false;
    m_lobbyButtonsCreated = false;
    m_joinConfirmCreated = false;
    m_startButtonCreated = false;

    ensureFont();
    SDL_StartTextInput();
}

void Menu::deactivate()
{
    m_active = false;
    SDL_StopTextInput();
}

void Menu::deactivate(Registry& registry)
{
    deactivate();
    if (m_joinButtonCreated) {
        registry.kill_entity(m_joinServerButton);
        m_joinButtonCreated = false;
    }
    if (m_lobbyButtonsCreated) {
        registry.kill_entity(m_createLobbyButton);
        registry.kill_entity(m_joinLobbyButton);
        m_lobbyButtonsCreated = false;
    }
    if (m_joinConfirmCreated) {
        registry.kill_entity(m_joinConfirmButton);
        m_joinConfirmCreated = false;
    }
    if (m_startButtonCreated) {
        registry.kill_entity(m_startButton);
        m_startButtonCreated = false;
    }
}

void Menu::handleEvent(const SDL_Event& e)
{
    if (!m_active)
        return;

    if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
        SDL_StartTextInput();
    }
    if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
        SDL_StopTextInput();
    }

    if (m_page == Page::Connect || m_page == Page::Join) {
        if (e.type == SDL_TEXTINPUT && m_inputFocused) {
            m_inputCode += e.text.text;
        }
        if (e.type == SDL_KEYDOWN && m_inputFocused) {
            if (e.key.keysym.sym == SDLK_BACKSPACE) {
                if (!m_inputCode.empty())
                    m_inputCode.pop_back();
            } else if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
                if (m_page == Page::Connect) {
                    m_requestConnect = true;
                }
            }
        }
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            const int mx = e.button.x;
            const int my = e.button.y;
            if (mx >= m_inputRect.x && mx <= m_inputRect.x + m_inputRect.w && my >= m_inputRect.y && my <= m_inputRect.y + m_inputRect.h) {
                m_inputFocused = true;
                SDL_StartTextInput();
            } else {
                m_inputFocused = false;
            }
        }
    }
}

void Menu::render(GraphicsManager& gfx, Registry& registry)
{
    gfx.clear(10, 10, 30, 255);

    if (m_page == Page::Connect)
        renderConnectPage(gfx, registry);
    else if (m_page == Page::Lobby)
        renderLobbyPage(gfx, registry);
    else if (m_page == Page::Join)
        renderJoinPage(gfx, registry);
    else if (m_page == Page::Start)
        renderStartPage(gfx, registry);

    gfx.present();
}

void Menu::renderConnectPage(GraphicsManager& gfx, Registry& registry)
{
    auto* renderer = gfx.getRenderer();

    gfx.setDrawColor(30, 30, 60, 255);
    SDL_RenderFillRect(renderer, &m_inputRect);
    gfx.setDrawColor(200, 200, 230, 255);
    SDL_RenderDrawRect(renderer, &m_inputRect);
    renderInputText(renderer);

    if (m_inputFocused) {
        Uint32 ticks = SDL_GetTicks();
        if ((ticks / 500) % 2 == 0) {
            int caretX = m_inputRect.x + 8;
            if (m_font && !m_inputCode.empty()) {
                int w = 0, h = 0;
                TTF_SizeUTF8(m_font, m_inputCode.c_str(), &w, &h);
                caretX += w;
            } else {
                caretX += static_cast<int>(m_inputCode.size()) * 10;
            }
            SDL_Rect caret { caretX, m_inputRect.y + 8, 2, m_inputRect.h - 16 };
            gfx.setDrawColor(255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &caret);
        }
    }

    if (!m_joinButtonCreated) {
        m_joinServerButton = factories::createButton(
            registry,
            static_cast<float>(m_connectBtnRect.x),
            static_cast<float>(m_connectBtnRect.y),
            static_cast<float>(m_connectBtnRect.w),
            static_cast<float>(m_connectBtnRect.h),
            "join_server");
        m_joinButtonCreated = true;
    }

    SDL_Color btnFill { 40, 160, 220, 230 };
    SDL_Color btnBorder { 255, 255, 255, 255 };
    if (registry.has<Button>(m_joinServerButton)) {
        auto& b = registry.get<Button>(m_joinServerButton);
        if (b.is_hovered)
            btnFill = SDL_Color { 60, 180, 240, 230 };
        if (b.was_pressed) {
            b.was_pressed = false;
            m_requestConnect = true;
        }
    }

    gfx.setDrawColor(btnFill.r, btnFill.g, btnFill.b, btnFill.a);
    SDL_RenderFillRect(renderer, &m_connectBtnRect);
    gfx.setDrawColor(btnBorder.r, btnBorder.g, btnBorder.b, btnBorder.a);
    SDL_RenderDrawRect(renderer, &m_connectBtnRect);
    if (m_font)
        renderTextCentered(renderer, m_connectBtnRect, "Join Server", SDL_Color { 255, 255, 255, 255 });
}

void Menu::renderLobbyPage(GraphicsManager& gfx, Registry& registry)
{
    auto* renderer = gfx.getRenderer();

    if (!m_lobbyButtonsCreated) {
        m_createLobbyButton = factories::createButton(
            registry,
            static_cast<float>(m_createBtnRect.x),
            static_cast<float>(m_createBtnRect.y),
            static_cast<float>(m_createBtnRect.w),
            static_cast<float>(m_createBtnRect.h),
            "create_lobby");
        m_joinLobbyButton = factories::createButton(
            registry,
            static_cast<float>(m_joinBtnRect.x),
            static_cast<float>(m_joinBtnRect.y),
            static_cast<float>(m_joinBtnRect.w),
            static_cast<float>(m_joinBtnRect.h),
            "join_lobby");
        m_lobbyButtonsCreated = true;
    }

    SDL_Color crFill { 80, 200, 120, 230 }, crBorder { 255, 255, 255, 255 };
    if (registry.has<Button>(m_createLobbyButton)) {
        auto& b = registry.get<Button>(m_createLobbyButton);
        if (b.is_hovered)
            crFill = SDL_Color { 100, 220, 140, 230 };
        if (b.was_pressed) {
            b.was_pressed = false;
            m_page = Page::Start;
            registry.kill_entity(m_createLobbyButton);
            registry.kill_entity(m_joinLobbyButton);
            m_lobbyButtonsCreated = false;
            m_requestCreate = true;
        }
    }
    gfx.setDrawColor(crFill.r, crFill.g, crFill.b, crFill.a);
    SDL_RenderFillRect(renderer, &m_createBtnRect);
    gfx.setDrawColor(crBorder.r, crBorder.g, crBorder.b, crBorder.a);
    SDL_RenderDrawRect(renderer, &m_createBtnRect);
    if (m_font)
        renderTextCentered(renderer, m_createBtnRect, "Create Lobby", SDL_Color { 255, 255, 255, 255 });

    SDL_Color jnFill { 40, 160, 220, 230 }, jnBorder { 255, 255, 255, 255 };
    if (registry.has<Button>(m_joinLobbyButton)) {
        auto& b = registry.get<Button>(m_joinLobbyButton);
        if (b.is_hovered)
            jnFill = SDL_Color { 60, 180, 240, 230 };
        if (b.was_pressed) {
            b.was_pressed = false;
            onJoint();
            registry.kill_entity(m_createLobbyButton);
            registry.kill_entity(m_joinLobbyButton);
            m_lobbyButtonsCreated = false;
        }
    }
    gfx.setDrawColor(jnFill.r, jnFill.g, jnFill.b, jnFill.a);
    SDL_RenderFillRect(renderer, &m_joinBtnRect);
    gfx.setDrawColor(jnBorder.r, jnBorder.g, jnBorder.b, jnBorder.a);
    SDL_RenderDrawRect(renderer, &m_joinBtnRect);
    if (m_font)
        renderTextCentered(renderer, m_joinBtnRect, "Join Lobby", SDL_Color { 255, 255, 255, 255 });
}

void Menu::renderStartPage(GraphicsManager& gfx, Registry& registry)
{
    auto* renderer = gfx.getRenderer();

    if (!m_startButtonCreated) {
        m_startButton = factories::createButton(
            registry,
            static_cast<float>(m_startBtnRect.x),
            static_cast<float>(m_startBtnRect.y),
            static_cast<float>(m_startBtnRect.w),
            static_cast<float>(m_startBtnRect.h),
            "start_game");
        m_startButtonCreated = true;
    }

    SDL_Color stFill { 200, 160, 40, 230 }, stBorder { 255, 255, 255, 255 };
    if (registry.has<Button>(m_startButton)) {
        auto& b = registry.get<Button>(m_startButton);
        if (b.is_hovered)
            stFill = SDL_Color { 220, 180, 60, 230 };
        if (b.was_pressed) {
            b.was_pressed = false;
            m_requestStart = true;
        }
    }

    gfx.setDrawColor(stFill.r, stFill.g, stFill.b, stFill.a);
    SDL_RenderFillRect(renderer, &m_startBtnRect);
    gfx.setDrawColor(stBorder.r, stBorder.g, stBorder.b, stBorder.a);
    SDL_RenderDrawRect(renderer, &m_startBtnRect);
    if (m_font)
        renderTextCentered(renderer, m_startBtnRect, "Start", SDL_Color { 255, 255, 255, 255 });
}

void Menu::renderJoinPage(GraphicsManager& gfx, Registry& registry)
{
    auto* renderer = gfx.getRenderer();

    gfx.setDrawColor(30, 30, 60, 255);
    SDL_RenderFillRect(renderer, &m_inputRect);
    gfx.setDrawColor(200, 200, 230, 255);
    SDL_RenderDrawRect(renderer, &m_inputRect);
    renderInputText(renderer);

    if (m_inputFocused) {
        Uint32 ticks = SDL_GetTicks();
        if ((ticks / 500) % 2 == 0) {
            int caretX = m_inputRect.x + 8;
            if (m_font && !m_inputCode.empty()) {
                int w = 0, h = 0;
                TTF_SizeUTF8(m_font, m_inputCode.c_str(), &w, &h);
                caretX += w;
            } else {
                caretX += static_cast<int>(m_inputCode.size()) * 10;
            }
            SDL_Rect caret { caretX, m_inputRect.y + 8, 2, m_inputRect.h - 16 };
            gfx.setDrawColor(255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &caret);
        }
    }

    if (!m_joinConfirmCreated) {
        m_joinConfirmButton = factories::createButton(
            registry,
            static_cast<float>(m_connectBtnRect.x),
            static_cast<float>(m_connectBtnRect.y),
            static_cast<float>(m_connectBtnRect.w),
            static_cast<float>(m_connectBtnRect.h),
            "join_lobby_confirm");
        m_joinConfirmCreated = true;
    }

    SDL_Color btnFill { 40, 160, 220, 230 };
    SDL_Color btnBorder { 255, 255, 255, 255 };
    if (registry.has<Button>(m_joinConfirmButton)) {
        auto& b = registry.get<Button>(m_joinConfirmButton);
        if (b.is_hovered)
            btnFill = SDL_Color { 60, 180, 240, 230 };
        if (b.was_pressed) {
            b.was_pressed = false;
            m_requestJoin = true;
        }
    }

    gfx.setDrawColor(btnFill.r, btnFill.g, btnFill.b, btnFill.a);
    SDL_RenderFillRect(renderer, &m_connectBtnRect);
    gfx.setDrawColor(btnBorder.r, btnBorder.g, btnBorder.b, btnBorder.a);
    SDL_RenderDrawRect(renderer, &m_connectBtnRect);
    if (m_font)
        renderTextCentered(renderer, m_connectBtnRect, "Join Lobby", SDL_Color { 255, 255, 255, 255 });
}

void Menu::ensureFont()
{
    if (m_font)
        return;
    if (TTF_WasInit() == 0) {
        if (TTF_Init() == -1) {
            return;
        }
    }

    std::string fontPath = findFontPath();
    if (!fontPath.empty()) {
        m_font = TTF_OpenFont(fontPath.c_str(), m_fontSize);
    } else
        std::cout << "font not found\n";
}

std::string Menu::findFontPath() const
{
    const char* font = "client/res/fonts/OpenSans-Medium.ttf";
    if (std::filesystem::exists(font))
        return std::string(font);
    return std::string();
}

void Menu::renderInputText(SDL_Renderer* renderer)
{
    if (!m_font)
        return;
    if (m_inputCode.empty())
        return;
    SDL_Surface* surf = TTF_RenderUTF8_Blended(m_font, m_inputCode.c_str(), m_textColor);
    if (!surf)
        return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    if (!tex) {
        SDL_FreeSurface(surf);
        return;
    }

    SDL_Rect dst { m_inputRect.x + 8, m_inputRect.y + (m_inputRect.h - surf->h) / 2, surf->w, surf->h };
    SDL_Rect clip { 0, 0, surf->w, surf->h };
    int maxWidth = m_inputRect.w - 16;
    if (dst.w > maxWidth) {
        clip.x = dst.w - maxWidth;
        clip.w = maxWidth;
        dst.w = maxWidth;
    }

    SDL_RenderSetClipRect(renderer, &m_inputRect);
    SDL_RenderCopy(renderer, tex, &clip, &dst);
    SDL_RenderSetClipRect(renderer, nullptr);

    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);
}

void Menu::renderTextCentered(SDL_Renderer* renderer, const SDL_Rect& rect, const std::string& text, SDL_Color color)
{
    if (!m_font || text.empty())
        return;
    SDL_Surface* surf = TTF_RenderUTF8_Blended(m_font, text.c_str(), color);
    if (!surf)
        return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    if (!tex) {
        SDL_FreeSurface(surf);
        return;
    }

    SDL_Rect dst { rect.x + (rect.w - surf->w) / 2, rect.y + (rect.h - surf->h) / 2, surf->w, surf->h };
    SDL_RenderCopy(renderer, tex, nullptr, &dst);
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);
}

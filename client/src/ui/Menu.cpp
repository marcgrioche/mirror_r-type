#include "Menu.hpp"
#include "../managers/GraphicsManager.hpp"
#include "../entities/button/CreateButton.hpp"
#include "components/AllComponents.hpp"

#include <filesystem>
#include <iostream>

void Menu::activate()
{
    m_active = true;
    m_inputFocused = true;
    ensureFont();
    SDL_StartTextInput();
}

void Menu::deactivate()
{
    m_active = false;
    SDL_StopTextInput();
}

// --- ajouté ---
void Menu::deactivate(Registry& registry)
{
    deactivate();
    if (m_joinButtonCreated) {
        registry.kill_entity(m_joinServerButton);
        m_joinButtonCreated = false;
    }
}

void Menu::handleEvent(const SDL_Event& e)
{
    if (!m_active) return;

    if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
        SDL_StartTextInput();
    }
    if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
        SDL_StopTextInput();
    }

    if (e.type == SDL_TEXTINPUT && m_inputFocused) {
        m_inputCode += e.text.text;
    }
    if (e.type == SDL_KEYDOWN && m_inputFocused) {
        if (e.key.keysym.sym == SDLK_BACKSPACE) {
            if (!m_inputCode.empty())
                m_inputCode.pop_back();
        } else if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
            m_requestStart = true;
        }
    }

    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        const int mx = e.button.x;
        const int my = e.button.y;
        if (mx >= m_buttonRect.x && mx <= m_buttonRect.x + m_buttonRect.w &&
            my >= m_buttonRect.y && my <= m_buttonRect.y + m_buttonRect.h) {
            m_requestStart = true;
        }
        if (mx >= m_inputRect.x && mx <= m_inputRect.x + m_inputRect.w &&
            my >= m_inputRect.y && my <= m_inputRect.y + m_inputRect.h) {
            m_inputFocused = true;
            SDL_StartTextInput();
        }
    }
}

void Menu::render(GraphicsManager& gfx, Registry& registry)
{
    auto* renderer = gfx.getRenderer();
    gfx.clear(10, 10, 30, 255);

    gfx.setDrawColor(30, 30, 60, 255);
    SDL_RenderFillRect(renderer, &m_inputRect);
    gfx.setDrawColor(200, 200, 230, 255);
    SDL_RenderDrawRect(renderer, &m_inputRect);
    renderInputText(renderer);

    bool textDrawn = false;
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
            static_cast<float>(m_buttonRect.x),
            static_cast<float>(m_buttonRect.y),
            static_cast<float>(m_buttonRect.w),
            static_cast<float>(m_buttonRect.h),
            "join_server"
        );
        m_joinButtonCreated = true;
    }

    SDL_Color btnFill { 40, 160, 220, 230 };
    SDL_Color btnBorder { 255, 255, 255, 255 };
    if (registry.has<Button>(m_joinServerButton)) {
        auto& b = registry.get<Button>(m_joinServerButton);
        if (b.is_hovered) {
            btnFill = SDL_Color { 60, 180, 240, 230 };
        }
        if (b.was_pressed) {
            b.was_pressed = false;
            m_requestStart = true;
        }
    }

    gfx.setDrawColor(btnFill.r, btnFill.g, btnFill.b, btnFill.a);
    SDL_RenderFillRect(renderer, &m_buttonRect);
    gfx.setDrawColor(btnBorder.r, btnBorder.g, btnBorder.b, btnBorder.a);
    SDL_RenderDrawRect(renderer, &m_buttonRect);

    if (m_font) {
        renderTextCentered(renderer, m_buttonRect, "Join Server", SDL_Color { 255, 255, 255, 255 });
        textDrawn = true;
    }

    if (!textDrawn) {
        int maxBoxes = std::min<int>(static_cast<int>(m_inputCode.size()), 30);
        for (int i = 0; i < maxBoxes; ++i) {
            SDL_Rect dot { m_inputRect.x + 10 + i * 12, m_inputRect.y + m_inputRect.h / 2 - 3, 6, 6 };
            gfx.setDrawColor(220, 220, 255, 255);
            SDL_RenderFillRect(renderer, &dot);
        }
    }

    gfx.present();
}

void Menu::ensureFont()
{
    if (m_font) return;
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
    if (!m_font) return;
    if (m_inputCode.empty()) return;
    SDL_Surface* surf = TTF_RenderUTF8_Blended(m_font, m_inputCode.c_str(), m_textColor);
    if (!surf) return;
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
    if (!m_font || text.empty()) return;
    SDL_Surface* surf = TTF_RenderUTF8_Blended(m_font, text.c_str(), color);
    if (!surf) return;
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
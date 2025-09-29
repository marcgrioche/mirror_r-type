#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class GraphicsManager;

class Menu {
public:
    void activate();
    void deactivate();
    void handleEvent(const SDL_Event& e);
    void render(GraphicsManager& gfx);
    ~Menu() = default; // Font lifetime tied to process; TTF kept initialized until app exit

    bool shouldStart() const { return m_requestStart; }
    void consumeStartSignal() { m_requestStart = false; }
    const std::string& code() const { return m_inputCode; }

private:
    bool m_active = false;
    bool m_requestStart = false;

    std::string m_inputCode;
    bool m_inputFocused = true;

    SDL_Rect m_inputRect { 800 / 2 - 200, 600 / 2 - 80, 400, 50 };
    SDL_Rect m_buttonRect { 800 / 2 - 100, 600 / 2 + 0, 200, 60 };

    // Text rendering
    TTF_Font* m_font = nullptr;
    int m_fontSize = 22;
    SDL_Color m_textColor { 240, 240, 255, 255 };

    void ensureFont();
    std::string findFontPath() const;
    void renderInputText(SDL_Renderer* renderer);
};
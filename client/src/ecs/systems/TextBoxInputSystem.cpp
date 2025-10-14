/*
** TextBoxInputSystem.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 3:13:29 PM 2025 jojo
** Last update Thu Oct 8 2:38:11 PM 2025 jojo
*/

#include "TextBoxInputSystem.hpp"
#include "Hitbox.hpp"
#include "Position.hpp"
#include "components/TextBox.hpp"
#include "components/TextBoxInput.hpp"

void textBoxInputSystem(Registry& registry, const SDL_Event& event)
{
    // Gestion du clic pour focus
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mx = event.button.x;
        int my = event.button.y;

        auto view = registry.view<TextBoxInput, Position, Hitbox>();
        bool anyFocused = false;

        for (auto it = view.begin(); it != view.end(); ++it) {
            Entity e = it.entity();
            TextBoxInput& input = registry.get<TextBoxInput>(e);
            Position& pos = registry.get<Position>(e);
            Hitbox& hitbox = registry.get<Hitbox>(e);

            // Test point-in-rect
            bool clicked = (mx >= pos.v.x && mx <= pos.v.x + hitbox.width && my >= pos.v.y && my <= pos.v.y + hitbox.height);

            if (clicked && input.isActive) {
                input.isFocused = true;
                input.cursorPosition = input.inputText.length(); // curseur à la fin
                anyFocused = true;

                // Active la saisie de texte SDL
                SDL_StartTextInput();
            } else {
                input.isFocused = false;
            }
        }

        // Si aucune textbox cliquée, désactive la saisie
        if (!anyFocused) {
            SDL_StopTextInput();
        }
    }

    // Gestion de la saisie de texte
    auto view = registry.view<TextBoxInput, TextBox>();
    for (auto it = view.begin(); it != view.end(); ++it) {
        Entity e = it.entity();
        TextBoxInput& input = registry.get<TextBoxInput>(e);
        TextBox& textBox = registry.get<TextBox>(e);

        if (!input.isFocused || !input.isActive)
            continue;

        if (event.type == SDL_TEXTINPUT) {
            // Ajoute le caractère saisi
            std::string newChar = event.text.text;
            if (input.inputText.length() < input.maxLength) {
                input.inputText.insert(input.cursorPosition, newChar);
                input.cursorPosition += newChar.length();
            }
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_BACKSPACE:
                if (input.cursorPosition > 0 && !input.inputText.empty()) {
                    input.inputText.erase(input.cursorPosition - 1, 1);
                    input.cursorPosition--;
                }
                break;

            case SDLK_DELETE:
                if (input.cursorPosition < input.inputText.length()) {
                    input.inputText.erase(input.cursorPosition, 1);
                }
                break;

            case SDLK_LEFT:
                if (input.cursorPosition > 0) {
                    input.cursorPosition--;
                }
                break;

            case SDLK_RIGHT:
                if (input.cursorPosition < input.inputText.length()) {
                    input.cursorPosition++;
                }
                break;

            case SDLK_HOME:
                input.cursorPosition = 0;
                break;

            case SDLK_END:
                input.cursorPosition = input.inputText.length();
                break;

            case SDLK_RETURN:
            case SDLK_KP_ENTER:
                // Valide l'input (tu peux émettre un événement ici)
                input.isFocused = false;
                SDL_StopTextInput();
                break;
            case SDLK_v:
                if (SDL_GetModState() & KMOD_CTRL) {
                    // Ctrl+V : Coller depuis le presse-papiers
                    if (SDL_HasClipboardText()) {
                        char* clipboardText = SDL_GetClipboardText();
                        if (clipboardText) {
                            std::string pasteText = clipboardText;

                            // Limite la longueur totale
                            size_t availableSpace = input.maxLength - input.inputText.length();
                            if (pasteText.length() > availableSpace) {
                                pasteText = pasteText.substr(0, availableSpace);
                            }

                            // Insère le texte à la position du curseur
                            input.inputText.insert(input.cursorPosition, pasteText);
                            input.cursorPosition += pasteText.length();

                            SDL_free(clipboardText);
                        }
                    }
                }
                break;
            }
        }

        // Met à jour l'affichage de la TextBox
        if (input.inputText.empty()) {
            textBox.text = input.placeholder;
            textBox.color = { 100, 100, 100, 255 }; // gris pour placeholder
        } else {
            // Ajoute le curseur si focused
            if (input.isFocused && input.showCursor) {
                std::string displayText = input.inputText;
                displayText.insert(input.cursorPosition, "|");
                textBox.text = displayText;
            } else {
                textBox.text = input.inputText;
            }
            textBox.color = input.isFocused ? input.focusColor : input.normalColor;
        }
    }
}

void textBoxInputUpdateSystem(Registry& registry, float deltaTime)
{
    auto view = registry.view<TextBoxInput>();

    for (auto it = view.begin(); it != view.end(); ++it) {
        Entity e = it.entity();
        TextBoxInput& input = registry.get<TextBoxInput>(e);

        if (input.isFocused) {
            // Animation du curseur clignotant
            input.cursorBlinkTimer += deltaTime;
            if (input.cursorBlinkTimer >= 0.5f) {
                input.showCursor = !input.showCursor;
                input.cursorBlinkTimer = 0.0f;
            }
        } else {
            input.showCursor = false;
            input.cursorBlinkTimer = 0.0f;
        }
    }
}

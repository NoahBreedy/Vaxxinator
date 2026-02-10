#include <iostream>
#include "IntroScreen.h"

#define STATE_NAME "IntroScreen"

IntroScreen::IntroScreen(StateMachine* machine): State(STATE_NAME, machine)
{
}

void IntroScreen::render() {
    SDL_SetRenderDrawColor(state_machine->renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(state_machine->renderer);

    if (text_textures.empty()) {
        return;
    }

    int screen_width, screen_height;
    SDL_RenderGetLogicalSize(state_machine->renderer, &screen_width, &screen_height);

    // Moves up scroll_speed pixles every second
    float scroll_offset = (elapsed_time / 1000.0f) * scroll_speed;

    for (size_t i = 0; i < text_textures.size(); ++i) {
        if (text_textures[i] == nullptr) continue;

        int text_width, text_height;
        SDL_QueryTexture(text_textures[i], nullptr, nullptr, &text_width, &text_height);

        // Calculate vertical position (start from bottom, move up)
        int y = screen_height - scroll_offset + (i * line_spacing);
        
        // Center horizontally
        int x = (screen_width - text_width) / 2;
        
        // Create destination rectangle
        SDL_Rect dest_rect = {x, y, text_width, text_height};
        
        SDL_RenderCopy(state_machine->renderer, text_textures[i], nullptr, &dest_rect);
    }
}

void IntroScreen::update() {
    // Allow skipping intro with spacebar 
    if(state_machine->event.type == SDL_KEYDOWN && state_machine->event.key.keysym.sym == SDLK_SPACE) {
        state_machine->transition("MainMenu");
    }

    // Check if intro is complete
    if (!text_textures.empty()) {
        int screen_height;
        SDL_RenderGetLogicalSize(state_machine->renderer, nullptr, &screen_height);
        
        float scroll_offset = (elapsed_time / 1000.0f) * scroll_speed;
        
        size_t last_index = text_textures.size() - 1;
        int last_text_height;
        SDL_QueryTexture(text_textures[last_index], nullptr, nullptr, nullptr, &last_text_height);
        
        int last_y = screen_height - scroll_offset + (last_index * line_spacing);
        
        if (last_y + last_text_height < 0) {
            state_machine->transition("MainMenu");
        }
    }

    SDL_RenderPresent(state_machine->renderer);
    SDL_Delay(10);
    elapsed_time += 10;
}

void IntroScreen::enter() {
    std::cout << "Entering " << STATE_NAME << std::endl;

    elapsed_time = 0;
    scroll_speed = 50.0f;
    line_spacing = 80.0f;
    font = TTF_OpenFont("assets/daydream.otf", 8);

    if (font == nullptr) {
        std::cerr << "Error: Failed to load font: " << TTF_GetError() << std::endl;
    }

    SDL_Color white = {255, 255, 255, 255};
    const char* text_lines[] = {
        "This is a test for the Vaxxinator",
        "introscreen text rendering1",
        "introscreen text rendering2",
        "introscreen text rendering3",
        "introscreen text rendering4"
    };

    for (const auto& line : text_lines) {
        SDL_Surface* text_surface = TTF_RenderText_Solid(font, line, white);
        if (text_surface == nullptr) {
            std::cerr << "Error: Failed to render text surface: " << TTF_GetError() << std::endl;
            continue;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(state_machine->renderer, text_surface);
        if (texture == nullptr) {
            std::cerr << "Error: Failed to create texture: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(text_surface);
            continue;
        }

        text_textures.push_back(texture);

        SDL_FreeSurface(text_surface);
    }
}

void IntroScreen::exit() {
    std::cout << "Exiting " << STATE_NAME << std::endl;
    if (font != nullptr) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    for (auto& texture : text_textures) {
        if (texture != nullptr) {
            SDL_DestroyTexture(texture);
        }
    }
    text_textures.clear();
}
#include <iostream>
#include "states/IntroScreen.h"

#define STATE_NAME "IntroScreen"

IntroScreen::IntroScreen(StateMachine* machine): State(STATE_NAME, machine)
{
}

void IntroScreen::render() {
    // Check if window size changed
    int current_width, current_height;
    SDL_GetRendererOutputSize(state_machine->renderer, &current_width, &current_height);
    
    if (current_width != last_window_width || current_height != last_window_height) {
        last_window_width = current_width;
        last_window_height = current_height;
        createTextTextures();  // Regenerate at new size
    }

    SDL_SetRenderDrawColor(state_machine->renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(state_machine->renderer);

    if (text_textures.empty()) {
        return;
    }

    int screen_width, screen_height;
    SDL_GetRendererOutputSize(state_machine->renderer, &screen_width, &screen_height);

    float scaled_line_spacing = line_spacing * state_machine->ui_scale;
    float scaled_scroll_speed = scroll_speed * std::sqrt(state_machine->ui_scale);

    // Moves up scroll_speed pixles every second
    float scroll_offset = (elapsed_time / 1000.0f) * scaled_scroll_speed;

    float base_hue = std::fmod(elapsed_time * 0.05f, 360.0f);

    for (size_t i = 0; i < text_textures.size(); ++i) {
        if (text_textures[i] == nullptr) continue;

        int text_width, text_height;
        SDL_QueryTexture(text_textures[i], nullptr, nullptr, &text_width, &text_height);

        // Calculate vertical position (start from bottom, move up)
        int y = screen_height - scroll_offset + (i * scaled_line_spacing);
        
        // Center horizontally
        int x = (screen_width - text_width) / 2;
        
        // Create destination rectangle
        SDL_Rect dest_rect = {x, y, text_width, text_height};

        float hue = fmod(base_hue + i * 20.0f, 360.0f);

        uint8_t r, g, b;
        hsv_to_rgb(hue, 1.0f, 1.0f, r, g, b);

        SDL_SetTextureColorMod(text_textures[i], r, g, b);
        
        SDL_RenderCopy(state_machine->renderer, text_textures[i], nullptr, &dest_rect);
    }

    SDL_RenderPresent(state_machine->renderer);
}

void IntroScreen::update() {
    // Allow skipping intro with spacebar 
    if (state_machine->input_buffer.has_key()) {
        SDL_Keycode key = state_machine->input_buffer.pop_key();

        if (key == SDLK_SPACE) {
            state_machine->transition("MainMenu");
        }
    }

    // Check if intro is complete
    if (!text_textures.empty()) {
        int screen_height;
        SDL_GetRendererOutputSize(state_machine->renderer, nullptr, &screen_height);
        
        float scroll_offset = (elapsed_time / 1000.0f) * scroll_speed;
        
        size_t last_index = text_textures.size() - 1;
        int last_text_height;
        SDL_QueryTexture(text_textures[last_index], nullptr, nullptr, nullptr, &last_text_height);
        
        int last_y = screen_height - scroll_offset + (last_index * line_spacing);
        
        if (last_y + last_text_height < 0) {
            state_machine->transition("MainMenu");
        }
    }

    SDL_Delay(10);
    elapsed_time += 10;
}

void IntroScreen::enter() {
    std::cout << "Entering " << STATE_NAME << std::endl;

    elapsed_time = 0;
    scroll_speed = 50.0f;
    line_spacing = 200.0f;
    
    SDL_GetRendererOutputSize(state_machine->renderer, &last_window_width, &last_window_height);
    
    createTextTextures();
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

void IntroScreen::createTextTextures() {
    // Clear old textures
    for (auto& texture : text_textures) {
        if (texture != nullptr) {
            SDL_DestroyTexture(texture);
        }
    }
    text_textures.clear();

    // Close old font if it exists
    if (font != nullptr) {
        TTF_CloseFont(font);
    }

    // Calculate scaled font size
    const int base_font_size = 48;
    int scaled_size = (int)(base_font_size * state_machine->ui_scale);
    font = TTF_OpenFont("assets/fonts/daydream.otf", scaled_size);

    if (font == nullptr) {
        std::cerr << "Error: Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    const char* text_lines[] = {
        "THE YEAR IS 1997",
        "THE EARTH HAS",
        "BECOME INFECTED",
        "WITH A DEATHLY AND",
        "DEADLY DISEASE!",
        "THE AMOS FOUNDATION",
        "IN PARTNERSHIP WITH",
        "TEENYINC HAS CREATED",
        "WHAT JUST MAY BE",
        "THE LAST HOPE FOR",
        "HUMANITY TO SURVIVE",
        "NANOSCOPIC AGENTS",
        "ARE PROGGRAMED FOR",
        "ONE THING...",
        "COMPLETE PATHOGEN",
        "EXTERMINATION",
        "WILL YOU AID THEM?",
        "CAN YOU BECOME A",
        " ",
        "VAXXINATOR",
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
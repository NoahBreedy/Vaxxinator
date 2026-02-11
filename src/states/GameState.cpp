#include <ctime>
#include <iostream>
#include "states/GameState.h"

#define STATE_NAME "GameState"
GameState::GameState(StateMachine* machine): State(STATE_NAME, machine)
{
    /* Setup texture for pixel modification */
    format = 0;
    pitch = 0;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    background_texture = SDL_CreateTexture(
        state_machine->renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        CANVAS_WIDTH,
        CANVAS_HEIGHT
    );
    SDL_QueryTexture(background_texture, &format , nullptr, nullptr, nullptr);
    SDL_PixelFormat *pixel_format = SDL_AllocFormat(format);

    /* For now I make the color map hold purple but we can randomize it... */
    for(int i = 0; i < 256; i++){
        color_map[i] = SDL_MapRGB(pixel_format,i,0,i);
    }
    
    pixels = nullptr;
    strand_size = 32;

    frame_count = 0;
}

void GameState::render() {
    SDL_SetRenderDrawColor(state_machine->renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(state_machine->renderer);

    /* Draw simplex noise background
     * to describe this algorithm for future Noah it basically 
     * calulates the simplex value based on the x & y coordinate of 
     * the texture from 2 different generators along with our frame 
     * count to get a sort of movement "animation. It then combines 
     * them at different proportions 1/15 and 2/15 to get ourfinal 
     * simplex value. We then just use this value to get an index into 
     * a colormap and the result is cool looking moving "veins"
     */
    SDL_LockTexture(background_texture, nullptr, &pixels, &pitch);
    uint32_t* target_pixels = (uint32_t*)pixels;
    int pixels_per_row = pitch / sizeof(uint32_t); 
    for (int y = 0; y < CANVAS_HEIGHT; ++y) {
            for (int x = 0; x < CANVAS_WIDTH; ++x) {
                double v = open_simplex_noise3(noise_ctx0 , (double)x / strand_size  /  1.0,(double) y  / strand_size / 1.0 , (double)frame_count   /   (8  * MOVEMENT_SCALER));
                double v2 = open_simplex_noise3(noise_ctx1, (double)x / strand_size  /  2.0,(double) y  / strand_size / 2.0 , (double)frame_count   /   (16 * MOVEMENT_SCALER));
                
                v = (v *  1.0  / 15.0) + (v2 * 2.0 / 15.0);

                uint8_t color_index  = 256.0 * std::abs(v);
                
                target_pixels[y * pixels_per_row + x] = color_map[color_index];
            }
    }
    SDL_UnlockTexture(background_texture); // Let the gpu take over...

    /* Blit the texture to the renderer */
    SDL_RenderCopy(state_machine->renderer, background_texture, nullptr, nullptr);
}

void GameState::update() {
    if (state_machine->input_buffer.has_key()) {
        SDL_Keycode key = state_machine->input_buffer.pop_key();

        if (key == SDLK_SPACE) {
            state_machine->transition("MainMenu");
        }
    }

    frame_count++;

    SDL_RenderPresent(state_machine->renderer);
    SDL_Delay(10);
}

void GameState::enter() {
    std::cout << "Entering " << STATE_NAME << std::endl;

    std::srand(std::time(nullptr));
    open_simplex_noise(std::rand(),&noise_ctx0);
    open_simplex_noise(std::rand(),&noise_ctx1);
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    background_texture = SDL_CreateTexture(
        state_machine->renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        CANVAS_WIDTH,
        CANVAS_HEIGHT
    );

    frame_count = 0;
}

void GameState::exit() {
    std::cout << "Exiting " << STATE_NAME << std::endl;

    open_simplex_noise_free(noise_ctx0);
    open_simplex_noise_free(noise_ctx1);
    SDL_DestroyTexture(background_texture);
    pixels = nullptr;
}

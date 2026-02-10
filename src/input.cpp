#include "input.h"
#include <cstring>

Input::Input() {
    std::memset(keys_down, 0, sizeof(keys_down));
}

void Input::update(SDL_Event& event) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit = true;
        }

        if (event.type == SDL_KEYDOWN) {
            keys_down[event.key.keysym.scancode] = true;

            if (event.key.repeat == 0) {
                key_buffer.push(event.key.keysym.sym);
            }
        }

        if (event.type == SDL_KEYUP) {
            keys_down[event.key.keysym.scancode] = false;
        }
    }
}

bool Input::has_key() const {
    return !key_buffer.empty();
}

SDL_Keycode Input::pop_key() {
    if (key_buffer.empty()) return SDLK_UNKNOWN;

    SDL_Keycode k = key_buffer.front();
    key_buffer.pop();
    return k;
}

void Input::clear_keys() {
    while (!key_buffer.empty()) key_buffer.pop();
}

bool Input::key_down(SDL_Scancode sc) const {
    return keys_down[sc];
}

bool Input::quit_requested() const {
    return quit;
}

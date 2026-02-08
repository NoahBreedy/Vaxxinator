#include "input.h"

std::queue<SDL_Keycode> key_buffer;

void update_key_buffer(SDL_Event& event) {
    while (SDL_PollEvent(&event)) {
        key_buffer.push(event.key.keysym.sym);
    }
}

bool has_key() {
    return !key_buffer.empty();
}

SDL_Keycode pop_key() {
    if (key_buffer.empty()) return SDLK_UNKNOWN;

    SDL_Keycode key = key_buffer.front();
    key_buffer.pop();
    return key;
}

void clear_keys() {
    while (!key_buffer.empty()) {
        key_buffer.pop();
    }
}

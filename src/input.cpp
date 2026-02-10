#include "input.h"
#include <cstring>

std::queue<SDL_Keycode> key_buffer;
bool quit_requested = false;

static bool g_keys_down[SDL_NUM_SCANCODES];

bool key_down(SDL_Scancode sc) {
    return g_keys_down[sc];
}

void update_key_buffer(SDL_Event& event) {
    static bool inited = false;
    if (!inited) {
        std::memset(g_keys_down, 0, sizeof(g_keys_down));
        inited = true;
    }

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit_requested = true;
        }

        if (event.type == SDL_KEYDOWN) {
            g_keys_down[event.key.keysym.scancode] = true;

            if (event.key.repeat == 0) {
                key_buffer.push(event.key.keysym.sym);
            }
        }

        if (event.type == SDL_KEYUP) {
            g_keys_down[event.key.keysym.scancode] = false;
        }
    }
}

bool has_key() {
    return !key_buffer.empty();
}

SDL_Keycode pop_key() {
    if (key_buffer.empty()) return SDLK_UNKNOWN;
    SDL_Keycode k = key_buffer.front();
    key_buffer.pop();
    return k;
}

void clear_keys() {
    while (!key_buffer.empty()) key_buffer.pop();
}

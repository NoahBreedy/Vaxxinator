#pragma once
#include <queue>
#include <SDL2/SDL.h>

class Input {
public:
    Input();

    void update(SDL_Event& event);

    bool has_key() const;
    SDL_Keycode pop_key();
    void clear_keys();

    bool key_down(SDL_Scancode sc) const;

    bool quit_requested() const;

private:
    std::queue<SDL_Keycode> key_buffer;
    bool quit = false;
    bool keys_down[SDL_NUM_SCANCODES];
};
#pragma once
#include <queue>
#include <SDL2/SDL.h>

extern std::queue<SDL_Keycode> key_buffer;
extern bool quit_requested;

void update_key_buffer(SDL_Event& event);

bool has_key();
SDL_Keycode pop_key();
void clear_keys();

bool key_down(SDL_Scancode sc);

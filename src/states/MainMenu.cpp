#include <iostream>
#include "states/MainMenu.h"

#define STATE_NAME "MainMenu"
MainMenu::MainMenu(StateMachine* machine): State(STATE_NAME, machine)
{
}

void MainMenu::render() {
    SDL_SetRenderDrawColor(state_machine->renderer, 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(state_machine->renderer);
}

void MainMenu::update() {
    if (state_machine->input_buffer.has_key()) {
        SDL_Keycode key = state_machine->input_buffer.pop_key();

        if (key == SDLK_SPACE) {
            state_machine->transition("GameState");
        }
    }

    SDL_RenderPresent(state_machine->renderer);
    SDL_Delay(10);
}

void MainMenu::enter() {
    std::cout << "Entering " << STATE_NAME << std::endl;
}

void MainMenu::exit() {
    std::cout << "Exiting " << STATE_NAME << std::endl;
}

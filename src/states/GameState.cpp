#include <iostream>
#include "GameState.h"

#define STATE_NAME "GameState"
GameState::GameState(StateMachine* machine): State(STATE_NAME, machine)
{
}

void GameState::render() {
    SDL_SetRenderDrawColor(state_machine->renderer, 0x00, 0x00, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(state_machine->renderer);
}

void GameState::update() {
    if (state_machine->input_buffer.has_key()) {
        SDL_Keycode key = state_machine->input_buffer.pop_key();

        if (key == SDLK_SPACE) {
            state_machine->transition("MainMenu");
        }
    }

    SDL_RenderPresent(state_machine->renderer);
    SDL_Delay(10);
}

void GameState::enter() {
    std::cout << "Entering " << STATE_NAME << std::endl;
}

void GameState::exit() {
    std::cout << "Exiting " << STATE_NAME << std::endl;
}

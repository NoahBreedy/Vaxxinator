#include <iostream>
#include "DemoState.h"

#define STATE_NAME "DemoState"
DemoState::DemoState(StateMachine* machine): State(STATE_NAME, machine)
{
}

void DemoState::render() {
    SDL_SetRenderDrawColor(state_machine->renderer, 0x00, 0x00, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(state_machine->renderer);
}

void DemoState::update() {
    SDL_Keycode key = state_machine->input_buffer.pop_key();
    if (state_machine->input_buffer.has_key()) {
        SDL_Keycode key = state_machine->input_buffer.pop_key();

        if (key == SDLK_SPACE) {
            state_machine->transition("MainMenu");
        }
    }

    SDL_RenderPresent(state_machine->renderer);
    //SDL_Delay(10);
}

void DemoState::enter() {
    std::cout << "Entering " << STATE_NAME << std::endl;
}

void DemoState::exit() {
    std::cout << "Exiting " << STATE_NAME << std::endl;
}

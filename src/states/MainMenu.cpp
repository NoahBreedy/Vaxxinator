#include <iostream>
#include "MainMenu.h"

#define STATE_NAME "MainMenu"
MainMenu::MainMenu(StateMachine* machine): State(STATE_NAME, machine)
{
}

void MainMenu::render() {
    SDL_SetRenderDrawColor(state_machine->renderer, 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(state_machine->renderer);
}

void MainMenu::update() {
    /* Someone should write a utility function for handling btn presses... */
    if(state_machine->event.type == SDL_KEYDOWN && state_machine->event.key.keysym.sym == SDLK_SPACE) {
            state_machine->transition("GameState");
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

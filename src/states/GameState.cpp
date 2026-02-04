#include <iostream>
#include "GameState.h"

#define STATE_NAME "GameState"
GameState::GameState(StateMachine* machine): State(STATE_NAME, machine)
{
}

void GameState::render() {
    SDL_FillRect(state_machine->surface, nullptr, SDL_MapRGB(state_machine->surface->format,0,0,255));
}

void GameState::update() {
    /* FOR REAL Someone should write a utility function for handling btn presses... */
    if(state_machine->event.type == SDL_KEYDOWN && state_machine->event.key.keysym.sym == SDLK_SPACE) {
            state_machine->transition("MainMenu");
    }
    SDL_UpdateWindowSurface(state_machine->window);
    SDL_Delay(10);
}

void GameState::enter() {
    std::cout << "Entering " << STATE_NAME << std::endl;
}

void GameState::exit() {
    std::cout << "Exiting " << STATE_NAME << std::endl;
}

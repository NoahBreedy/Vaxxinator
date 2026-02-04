#include <iostream>
#include "MainMenu.h"

#define STATE_NAME "MainMenu"
MainMenu::MainMenu(StateMachine* machine): State(STATE_NAME, machine)   
{
}

void MainMenu::render() {
    SDL_FillRect(state_machine->surface, nullptr, SDL_MapRGB(state_machine->surface->format,255,0,0));   
}

void MainMenu::update() {
    /* Someone should write a utility function for handling btn presses... */
    if(state_machine->event.type == SDL_KEYDOWN && state_machine->event.key.keysym.sym == SDLK_SPACE) {
            state_machine->transition("GameState");
    }
    SDL_UpdateWindowSurface(state_machine->window); 
    SDL_Delay(10);
}

void MainMenu::enter() {
    std::cout << "Entering " << STATE_NAME << std::endl;
}

void MainMenu::exit() {
    std::cout << "Exiting " << STATE_NAME << std::endl;
}

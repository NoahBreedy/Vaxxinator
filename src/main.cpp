#include <iostream>
#include "StateMachine.h"

int main(int argc, char* argv[]) {
    StateMachine application;

    if(!application.init()) {
        return EXIT_FAILURE;
    }

    while(SDL_PollEvent(&application.event) == 0 || application.event.type != SDL_QUIT){
        application.current_state->render();
        application.current_state->update();
    }

    return EXIT_SUCCESS;
}
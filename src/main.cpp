#include <iostream>
#include "StateMachine.h"

#include "input.h"

int main(int argc, char* argv[]) {
    StateMachine application;
    SDL_Event event;

    if(!application.init()) {
        return EXIT_FAILURE;
    }

    while(SDL_PollEvent(&application.event) == 0 || application.event.type != SDL_QUIT){
        update_key_buffer(event);
        application.current_state->render();
        application.current_state->update();
    }

    return EXIT_SUCCESS;
}
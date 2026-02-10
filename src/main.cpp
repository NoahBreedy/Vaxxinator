#include <iostream>
#include "core/StateMachine.h"

#include "core/Input.h"

int main(int argc, char* argv[]) {
    StateMachine application;

    if(!application.init()) {
        return EXIT_FAILURE;
    }

    while(!application.input_buffer.quit_requested()){
        application.input_buffer.update(application.event);
        application.current_state->render();
        application.current_state->update();
    }

    return EXIT_SUCCESS;
}
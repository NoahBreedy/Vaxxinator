#include <iostream>
#include <SDL.h>
#include "StateMachine.h"

int main(int argc, char* argv[]) {
    StateMachine application;
    application.init();
    return EXIT_SUCCESS;
}
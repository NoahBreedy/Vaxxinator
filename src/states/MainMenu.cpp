#include <iostream>
#include "MainMenu.h"

#define STATE_NAME "MainMenu"
MainMenu::MainMenu(StateMachine* machine): State(STATE_NAME, machine)   
{
}

void MainMenu::render() {
    std::cout << "Rendering " << STATE_NAME << std::endl;
}

void MainMenu::update() {
    std::cout << "Updating " << STATE_NAME << std::endl;
}

void MainMenu::enter() {
    std::cout << "Entering " << STATE_NAME << std::endl;
}

void MainMenu::exit() {
    std::cout << "Exiting " << STATE_NAME << std::endl;
}

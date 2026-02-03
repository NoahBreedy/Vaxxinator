#include <iostream>
#include "StateMachine.h"
#include "states/MainMenu.h"

/* Add your states to this fucntion if you want them to be recognized */
void StateMachine::init() {
    add(std::make_unique<MainMenu>(this));
    
    transition("MainMenu"); // start at main menu state?
                            // maybe add a splash screen state later? lmk
}

void StateMachine::add(std::unique_ptr<State> state) {
    state_map.emplace(state->getName(), std::move(state));
}

void StateMachine::transition(const std::string& state_name) {
    auto state = state_map.find(state_name);

    if (state == state_map.end()) {
        std::cout << "Invalid state_name failed transition" << std::endl;
        return;
    }

    if (current_state)
        current_state->exit();

    current_state = state->second.get();
    current_state->enter();

}
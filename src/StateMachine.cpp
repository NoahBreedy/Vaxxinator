#include <iostream>
#include "StateMachine.h"
#include "MainMenu.h"
#include "GameState.h"
#include "DemoState.h"

/* SDL2 gets setup here and then we build our states */
bool StateMachine::init() {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        std::cout << "SDL_Init() fail... " << SDL_GetError() << std::endl;
        return false;
    }

    /* Create the window and surface to render to */
    window = SDL_CreateWindow("Vaxxinator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                            CANVAS_WIDTH, CANVAS_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    
    /* Enable hardware acceleration for the renderer and allow integer scaling */
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, CANVAS_WIDTH, CANVAS_HEIGHT);
    SDL_RenderSetIntegerScale(renderer, SDL_TRUE);

    init_states();

    application_initialized = true;

    return true;
}

/* Add your states to this fucntion if you want them to be recognized */
void StateMachine::init_states() {
    add(std::make_unique<MainMenu>(this));
    add(std::make_unique<GameState>(this));
    add(std::make_unique<DemoState>(this));

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

StateMachine::~StateMachine() {
    if(application_initialized) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }
    /* we can safely call SDL_Quit even if SDL_init fails */
    SDL_Quit();
}
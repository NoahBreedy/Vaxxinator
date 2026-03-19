#include <iostream>
#include <SDL_ttf.h>
#include "core/StateMachine.h"
#include "states/MainMenu.h"
#include "states/GameState.h"
#include "states/IntroScreen.h"
#include "states/DemoState.h"

static void handleClayError(Clay_ErrorData error) {
    std::cerr << "[Clay] " << error.errorText.chars << std::endl;
}

static Clay_Dimensions measureText(Clay_StringSlice text,
                                   Clay_TextElementConfig* config,
                                   void* userData) {
    return SDL2_MeasureText(text, config, userData);
}
// Event watch callback for window resize events
static int windowEventWatch(void* userdata, SDL_Event* event) {
    StateMachine* state_machine = static_cast<StateMachine*>(userdata);
    
    if (event->type == SDL_WINDOWEVENT) {
        if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
            int width = event->window.data1;
            int height = event->window.data2;
            state_machine->window_width = width;
            state_machine->window_height = height;
            state_machine->update_ui_scale();

            if(state_machine->current_state->getName() != "GameState") {
                SDL_RenderSetLogicalSize(state_machine->renderer, width, height);
            }
        }
    }
    
    return 1;
}

/* SDL2 gets setup here and then we build our states */
bool StateMachine::init() {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        std::cout << "SDL_Init() fail... " << SDL_GetError() << std::endl;
        return false;
    }

    if(TTF_Init() < 0) {
        std::cout << "TTF_Init() fail... " << TTF_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    /* Create the window and surface to render to */
    window = SDL_CreateWindow("Vaxxinator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                            CANVAS_WIDTH, CANVAS_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    
    /* Enable hardware acceleration for the renderer and allow integer scaling */
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Register event watch for window resize events
    SDL_AddEventWatch(windowEventWatch, this);

    init_clay();
    init_states();

    application_initialized = true;

    return true;
}

/* Add your states to this fucntion if you want them to be recognized */
void StateMachine::init_states() {
    add(std::make_unique<MainMenu>(this));
    add(std::make_unique<GameState>(this));
    add(std::make_unique<IntroScreen>(this));
    add(std::make_unique<DemoState>(this));

    //transition("MainMenu"); // start at main menu state?
                            // maybe add a splash screen state later? lmk

    transition("IntroScreen");
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

void StateMachine::init_clay() {
    // Load the font
    fonts[FONT_BODY].fontId = FONT_BODY;
    fonts[FONT_BODY].font   = TTF_OpenFont("assets/fonts/PressStart2P.ttf", 48);
    if (!fonts[FONT_BODY].font) {
        std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << std::endl;
    }

    // Allocate Clay's memory arena
    uint64_t memorySize = Clay_MinMemorySize();
    clay_memory = malloc(memorySize);
    clay_arena = Clay_CreateArenaWithCapacityAndMemory(memorySize, clay_memory);

    // Initialize Clay
    Clay_Initialize(
        clay_arena,
        { (float)window_width, (float)window_height },
        { handleClayError }
    );
    Clay_SetMeasureTextFunction(measureText, fonts);
}

void StateMachine::cleanup_clay() {
    if (fonts[FONT_BODY].font) {
        TTF_CloseFont(fonts[FONT_BODY].font);
        fonts[FONT_BODY].font = nullptr;
    }
    free(clay_memory);
    clay_memory = nullptr;
}

StateMachine::~StateMachine() {
    if(application_initialized) {
        cleanup_clay();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }
    /* we can safely call SDL_Quit even if SDL_init fails */
    TTF_Quit();
    SDL_Quit();
}
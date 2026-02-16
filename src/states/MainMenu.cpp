#include <iostream>
#include "states/MainMenu.h"

#define STATE_NAME "MainMenu"

MainMenu::MainMenu(StateMachine* machine): State(STATE_NAME, machine)
{
}

void MainMenu::render()
{
    SDL_SetRenderDrawColor(state_machine->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state_machine->renderer);

    // Keep Clay in sync with the window size each frame
    Clay_SetLayoutDimensions((Clay_Dimensions){
        (float)CANVAS_WIDTH,
        (float)CANVAS_HEIGHT
    });

    buildLayout();

    Clay_SDL2_Render(state_machine->renderer, state_machine->clay_render_commands, state_machine->fonts);
    SDL_RenderPresent(state_machine->renderer);
}
void MainMenu::update() {
    if (state_machine->input_buffer.has_key()) {
        SDL_Keycode key = state_machine->input_buffer.pop_key();

        if (key == SDLK_SPACE) {
            state_machine->transition("GameState");
        }
    }

    SDL_Delay(10);
}

void MainMenu::enter() {
    std::cout << "Entering " << STATE_NAME << std::endl;
    // Clay is now initialized in StateMachine::init()
}

void MainMenu::exit() {
    std::cout << "Exiting " << STATE_NAME << std::endl;
    // Clay cleanup is handled by StateMachine::~StateMachine()
}

void MainMenu::buildLayout()
{
    int mouseX = 0, mouseY = 0;
    SDL_GetMouseState(&mouseX, &mouseY);
    Clay_SetPointerState(
        (Clay_Vector2){ (float)mouseX, (float)mouseY },
        SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(1)
    );

    Clay_BeginLayout();

    CLAY(CLAY_ID("Screen"), {
        .layout = {                             // layout FIRST
            .sizing = {
                .width  = CLAY_SIZING_GROW(0),
                .height = CLAY_SIZING_GROW(0)
            },
            .childAlignment = {
                .x = CLAY_ALIGN_X_CENTER,
                .y = CLAY_ALIGN_Y_CENTER
            }
        },
        .backgroundColor = { 15, 15, 30, 255 } // backgroundColor SECOND
    }) {
        CLAY_TEXT(CLAY_STRING("Vaxxinator"), CLAY_TEXT_CONFIG({
            .textColor = { 220, 220, 255, 255 }, // textColor FIRST
            .fontId    = FONT_BODY,               // fontId SECOND
            .fontSize  = 72                       // fontSize THIRD
        }));
    }

    state_machine->clay_render_commands = Clay_EndLayout();
}
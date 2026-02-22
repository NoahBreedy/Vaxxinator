#include <iostream>
#include "states/MainMenu.h"

#define STATE_NAME "MainMenu"

MainMenu::MainMenu(StateMachine* machine): State(STATE_NAME, machine)
{
}

// Callback function for button clicks
void MainMenu::injectButtonClicked(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    MainMenu* menu = (MainMenu*)userData;
    
    if (pointerData.state == CLAY_POINTER_DATA_RELEASED_THIS_FRAME) {
        menu->state_machine->transition("GameState");
    }
}

void MainMenu::onSyringeClicked(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    int syringeIndex = (int)(uintptr_t)userData;

    if (pointerData.state == CLAY_POINTER_DATA_RELEASED_THIS_FRAME) {
        std::cout << "Syringe " << syringeIndex << " clicked!" << std::endl;

        nfdchar_t* filePath = openFileDialog();

        if (filePath) {
            std::cout << "Selected file: " << filePath << std::endl;
            free(filePath); // Free the file path memory allocated by NF
        } else {
            std::cout << "No file selected." << std::endl;
        }
    }
}

void MainMenu::render()
{
    SDL_SetRenderDrawColor(state_machine->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state_machine->renderer);

    // Keep Clay in sync with the current window size each frame
    Clay_SetLayoutDimensions((Clay_Dimensions){
        (float)state_machine->window_width,
        (float)state_machine->window_height
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
}

void MainMenu::exit() {
    std::cout << "Exiting " << STATE_NAME << std::endl;
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

    const int base_title_size = 72;
    int scaled_title_size = (int)(base_title_size * state_machine->ui_scale);

    CLAY(CLAY_ID("Screen"), {
        .layout = {
            .sizing = {
                .width  = CLAY_SIZING_GROW(0),
                .height = CLAY_SIZING_GROW(0)
            },
            .padding = CLAY_PADDING_ALL(20),
            .childAlignment = {
                .x = CLAY_ALIGN_X_CENTER,
                .y = CLAY_ALIGN_Y_TOP
            },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = { 15, 15, 30, 255 }
    }) {

        // Title at upper center
        CLAY_TEXT(CLAY_STRING("Vaxxinator"), CLAY_TEXT_CONFIG({
            .textColor = { 220, 220, 255, 255 },
            .fontId    = FONT_BODY,
            .fontSize  = (uint16_t)scaled_title_size
        }));

        // Syringe row
        CLAY(CLAY_ID("SyringeRow"), {
            .layout = {
                .sizing = {
                    .width  = CLAY_SIZING_GROW(0),
                    .height = CLAY_SIZING_FIT(0)
                },
                .childGap = 20,
                .childAlignment = {
                    .x = CLAY_ALIGN_X_CENTER,
                    .y = CLAY_ALIGN_Y_CENTER
                },
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            }
        }) {
            CLAY(CLAY_ID("Syringe_0"), {
                .layout = {
                    .sizing = {
                        .width  = CLAY_SIZING_FIXED(80),
                        .height = CLAY_SIZING_FIXED(200)
                    }
                },
                .backgroundColor = Clay_Hovered()
                    ? (Clay_Color){ 80, 80, 100, 255 }
                    : (Clay_Color){ 50, 50, 70,  255 },
                .cornerRadius = CLAY_CORNER_RADIUS(4)
            }) {
                Clay_OnHover(onSyringeClicked, (void*)(uintptr_t)0);
            }

            CLAY(CLAY_ID("Syringe_1"), {
                .layout = {
                    .sizing = {
                        .width  = CLAY_SIZING_FIXED(80),
                        .height = CLAY_SIZING_FIXED(200)
                    }
                },
                .backgroundColor = Clay_Hovered()
                    ? (Clay_Color){ 80, 80, 100, 255 }
                    : (Clay_Color){ 50, 50, 70,  255 },
                .cornerRadius = CLAY_CORNER_RADIUS(4)
            }) {
                Clay_OnHover(onSyringeClicked, (void*)(uintptr_t)1);
            }

            CLAY(CLAY_ID("Syringe_2"), {
                .layout = {
                    .sizing = {
                        .width  = CLAY_SIZING_FIXED(80),
                        .height = CLAY_SIZING_FIXED(200)
                    }
                },
                .backgroundColor = Clay_Hovered()
                    ? (Clay_Color){ 80, 80, 100, 255 }
                    : (Clay_Color){ 50, 50, 70,  255 },
                .cornerRadius = CLAY_CORNER_RADIUS(4)
            }) {
                Clay_OnHover(onSyringeClicked, (void*)(uintptr_t)2);
            }

            CLAY(CLAY_ID("Syringe_3"), {
                .layout = {
                    .sizing = {
                        .width  = CLAY_SIZING_FIXED(80),
                        .height = CLAY_SIZING_FIXED(200)
                    }
                },
                .backgroundColor = Clay_Hovered()
                    ? (Clay_Color){ 80, 80, 100, 255 }
                    : (Clay_Color){ 50, 50, 70,  255 },
                .cornerRadius = CLAY_CORNER_RADIUS(4)
            }) {
                Clay_OnHover(onSyringeClicked, (void*)(uintptr_t)3);
            }
        }

        // Spacer to push button to bottom
        CLAY(CLAY_ID("Spacer"), {
            .layout = {
                .sizing = {
                    .width  = CLAY_SIZING_GROW(0),
                    .height = CLAY_SIZING_GROW(0)
                }
            }
        }) {}

        // Inject button at bottom center
        CLAY(CLAY_ID("Inject_Button"), {
            .layout = {
                .sizing = {
                    .width  = CLAY_SIZING_FIT(0),
                    .height = CLAY_SIZING_FIT(0)
                },
                .padding = { 12, 12, 6, 6 },
                .childAlignment = {
                    .x = CLAY_ALIGN_X_CENTER,
                    .y = CLAY_ALIGN_Y_CENTER
                }
            },
            .backgroundColor = Clay_Hovered()
                ? (Clay_Color){ 100, 150, 255, 255 }
                : (Clay_Color){  50, 100, 200, 255 },
            .cornerRadius = CLAY_CORNER_RADIUS(5)
        }) {
            CLAY_TEXT(CLAY_STRING("Inject"), CLAY_TEXT_CONFIG({
                .textColor = { 255, 255, 255, 255 },
                .fontId    = FONT_BODY,
                .fontSize  = (uint16_t)(scaled_title_size * 0.4f)
            }));
            Clay_OnHover(injectButtonClicked, (void*)this);
        }
    }

    state_machine->clay_render_commands = Clay_EndLayout();
}
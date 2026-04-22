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
        menu->state_machine->audio_mixer.play_audio("assets/audio/synth.wav");
        menu->state_machine->transition("GameState");
    }
}

void MainMenu::onSyringeClicked(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    MainMenu* menu = ((SyringeData*)userData)->userData;
    int i = ((SyringeData*)userData)->index;
   
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        menu->state_machine->audio_mixer.play_audio("assets/audio/blip.wav");
        nfdchar_t* filePath = openFileDialog();

        if (filePath) {
            menu->syringe_loaded[i] = true;
            menu->state_machine->syringe_paths[i] = std::string(filePath);
            free(filePath); // Remember to free the duplicated string after use
        } else {
            std::cout << "No file selected." << std::endl;
            menu->syringe_loaded[i] = false;
            menu->state_machine->syringe_paths[i] = std::string("");
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

    // Pre-pass: draw syringe sprites into Clay bounding boxes
    Clay_ElementId ids[4] = {
        Clay_GetElementId(CLAY_STRING("Syringe_0")),
        Clay_GetElementId(CLAY_STRING("Syringe_1")),
        Clay_GetElementId(CLAY_STRING("Syringe_2")),
        Clay_GetElementId(CLAY_STRING("Syringe_3"))
    };

    for (int i = 0; i < 4; i++) {
        Clay_ElementData data = Clay_GetElementData(ids[i]);

        if (data.found) {
            Clay_BoundingBox bb = data.boundingBox;
            SDL_Rect dst = {
                (int)bb.x,
                (int)bb.y,
                (int)bb.width,
                (int)bb.height
            };

            bool hovered = Clay_PointerOver(ids[i]);
            bool loaded  = syringe_loaded[i];

            const SDL_Rect* src;
            if      ( loaded &&  hovered) src = &SYRINGE_FULL_HOVERED;
            else if ( loaded && !hovered) src = &SYRINGE_FULL;
            else if (!loaded &&  hovered) src = &SYRINGE_EMPTY_HOVERED;
            else                          src = &SYRINGE_EMPTY;

            SDL_RenderCopy(state_machine->renderer, syringe_sheet, src, &dst);
        }
    }

    SDL_RenderPresent(state_machine->renderer);
}
void MainMenu::update() {
    if (state_machine->input_buffer.has_key()) {
        SDL_Keycode key = state_machine->input_buffer.pop_key();

        if (key == SDLK_SPACE) {
            state_machine->transition("GameState");
        }
    }
}

void MainMenu::enter() {
    syringe_data[0] = { 0, this };
    syringe_data[1] = { 1, this };
    syringe_data[2] = { 2, this };
    syringe_data[3] = { 3, this };
    
    SDL_Surface* surface = IMG_Load("assets/sprites/robot_syringe_modified.png");
    syringe_sheet = SDL_CreateTextureFromSurface(state_machine->renderer, surface);
    if (!syringe_sheet) {
        std::cerr << "Failed to load syringe texture: " << IMG_GetError() << std::endl;
        return;
    }
    SDL_FreeSurface(surface);
}

void MainMenu::exit() {
    state_machine->PLAYER_COUNT = 0;

    for (int i = 0; i < 4; i++){
        if(syringe_loaded[i]) {
            state_machine->PLAYER_COUNT++;
        }
    }

    if (syringe_sheet) {
        SDL_DestroyTexture(syringe_sheet);
        syringe_sheet = nullptr;
    }
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

    float scaled_syringe_width = (160 * state_machine->ui_scale);
    float scaled_syringe_height = (400 * state_machine->ui_scale);

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

        CLAY(CLAY_ID("Spacer_0"), {
            .layout = {
                .sizing = {
                    .width  = CLAY_SIZING_GROW(0),
                    .height = CLAY_SIZING_GROW(0)
                }
            }
        }) {}

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
                        .width  = CLAY_SIZING_FIXED(scaled_syringe_width),
                        .height = CLAY_SIZING_FIXED(scaled_syringe_height)
                    }
                },
                .backgroundColor = { 0, 0, 0, 0 },
                .cornerRadius = CLAY_CORNER_RADIUS(0)
            }) {
                Clay_OnHover(onSyringeClicked, (void*)&syringe_data[0]);
            }

            CLAY(CLAY_ID("Syringe_1"), {
                .layout = {
                    .sizing = {
                        .width  = CLAY_SIZING_FIXED(scaled_syringe_width),
                        .height = CLAY_SIZING_FIXED(scaled_syringe_height)
                    }
                },
                .backgroundColor = { 0, 0, 0, 0 },
                .cornerRadius = CLAY_CORNER_RADIUS(0)
            }) {
                Clay_OnHover(onSyringeClicked, (void*)&syringe_data[1]);
            }

            CLAY(CLAY_ID("Syringe_2"), {
                .layout = {
                    .sizing = {
                        .width  = CLAY_SIZING_FIXED(scaled_syringe_width),
                        .height = CLAY_SIZING_FIXED(scaled_syringe_height)
                    }
                },
                .backgroundColor = { 0, 0, 0, 0 },
                .cornerRadius = CLAY_CORNER_RADIUS(0)
            }) {
                Clay_OnHover(onSyringeClicked, (void*)&syringe_data[2]);
            }

            CLAY(CLAY_ID("Syringe_3"), {
                .layout = {
                    .sizing = {
                        .width  = CLAY_SIZING_FIXED(scaled_syringe_width),
                        .height = CLAY_SIZING_FIXED(scaled_syringe_height)
                    }
                },
                .backgroundColor = { 0, 0, 0, 0 },
                .cornerRadius = CLAY_CORNER_RADIUS(0)
            }) {
                Clay_OnHover(onSyringeClicked, (void*)&syringe_data[3]);
            }
        }

        // Spacer to push button to bottom
        CLAY(CLAY_ID("Spacer_1"), {
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
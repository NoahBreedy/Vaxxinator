#include <algorithm>
#include "states/GameOver.h"

#define STATE_NAME "GameOver"

// Medal colors for placements
static const Clay_Color GOLD_COLOR   = { 255, 215,   0, 255 };
static const Clay_Color SILVER_COLOR = { 192, 192, 192, 255 };
static const Clay_Color BRONZE_COLOR = { 205, 127,  50, 255 };
static const Clay_Color PLAIN_COLOR  = { 180, 180, 180, 255 };

static Clay_Color placementColor(int placement) {
    switch (placement) {
        case 1: return GOLD_COLOR;
        case 2: return SILVER_COLOR;
        case 3: return BRONZE_COLOR;
        default: return PLAIN_COLOR;
    }
}

GameOver::GameOver(StateMachine* machine) : State(STATE_NAME, machine)
{
}

void GameOver::mainMenuButtonClicked(Clay_ElementId elementId, Clay_PointerData pointerData, void* userData) {
    GameOver* go = (GameOver*)userData;
    if (pointerData.state == CLAY_POINTER_DATA_RELEASED_THIS_FRAME) {
        go->state_machine->transition("MainMenu");
    }
}

void GameOver::render() {
    SDL_SetRenderDrawColor(state_machine->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state_machine->renderer);

    Clay_SetLayoutDimensions((Clay_Dimensions){
        (float)state_machine->window_width,
        (float)state_machine->window_height
    });

    buildLayout();

    Clay_SDL2_Render(state_machine->renderer, state_machine->clay_render_commands, state_machine->fonts);

    SDL_RenderPresent(state_machine->renderer);
}

void GameOver::update() {
    if (state_machine->input_buffer.has_key()) {
        SDL_Keycode key = state_machine->input_buffer.pop_key();

        if (key == SDLK_SPACE || key == SDLK_RETURN) {
            state_machine->transition("MainMenu");
        }
    }
}

void GameOver::enter() {
    results.clear();

    for (int i = 0; i < state_machine->PLAYER_COUNT; i++) {
        PlayerResult r;
        r.score     = state_machine->player_scores[i];
        r.placement = 0;
        results.push_back(r);
    }

    // Sort descending by score
    std::sort(results.begin(), results.end(), [](const PlayerResult& a, const PlayerResult& b) {
        return a.score > b.score;
    });

    // Assign placements (ties get the same placement)
    for (int i = 0; i < (int)results.size(); i++) {
        if (i == 0) {
            results[i].placement = 1;
        } else if (results[i].score == results[i - 1].score) {
            results[i].placement = results[i - 1].placement;
        } else {
            results[i].placement = i + 1;
        }
    }
}

void GameOver::exit() {
    results.clear();
    state_machine->current_level = -1;
    for(int p = 0; p < 4; p++) {
        state_machine->player_scores[p] = 0;
    }
}

void GameOver::buildLayout() {
    int mouseX = 0, mouseY = 0;
    SDL_GetMouseState(&mouseX, &mouseY);
    Clay_SetPointerState(
        (Clay_Vector2){ (float)mouseX, (float)mouseY },
        SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(1)
    );

    Clay_BeginLayout();

    const int base_title_size  = 72;
    const int base_score_size  = 36;
    const int base_label_size  = 28;
    const int base_button_size = 32;

    int scaled_title_size  = (int)(base_title_size  * state_machine->ui_scale);
    int scaled_score_size  = (int)(base_score_size  * state_machine->ui_scale);
    int scaled_label_size  = (int)(base_label_size  * state_machine->ui_scale);
    int scaled_button_size = (int)(base_button_size * state_machine->ui_scale);

    float card_width  = 280 * state_machine->ui_scale;
    float card_height = 180 * state_machine->ui_scale;

    CLAY(CLAY_ID("Screen"), {
        .layout = {
            .sizing = {
                .width  = CLAY_SIZING_GROW(0),
                .height = CLAY_SIZING_GROW(0)
            },
            .padding           = CLAY_PADDING_ALL(30),
            .childGap          = (uint16_t)(24 * state_machine->ui_scale),
            .childAlignment    = {
                .x = CLAY_ALIGN_X_CENTER,
                .y = CLAY_ALIGN_Y_TOP
            },
            .layoutDirection   = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = { 10, 10, 25, 255 }
    }) {

        // Title
        CLAY_TEXT(CLAY_STRING("Game Over"), CLAY_TEXT_CONFIG({
            .textColor = { 220, 80, 80, 255 },
            .fontId    = FONT_BODY,
            .fontSize  = (uint16_t)scaled_title_size
        }));

        // Score cards row
        CLAY(CLAY_ID("ScoreRow"), {
            .layout = {
                .sizing = {
                    .width  = CLAY_SIZING_GROW(0),
                    .height = CLAY_SIZING_FIT(0)
                },
                .childGap       = (uint16_t)(20 * state_machine->ui_scale),
                .childAlignment = {
                    .x = CLAY_ALIGN_X_CENTER,
                    .y = CLAY_ALIGN_Y_CENTER
                },
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            }
        }) {
            for (int i = 0; i < (int)results.size(); i++) {
                Clay_Color accent = placementColor(results[i].placement);

                // Build runtime Clay_String for the placement label ("1st", "2nd", etc.)
                static const char* placement_strs[4] = { "1st", "2nd", "3rd", "4th" };
                int clamp = results[i].placement - 1;
                if (clamp < 0) clamp = 0;
                if (clamp > 3) clamp = 3;
                const char* plabel = placement_strs[clamp];
                Clay_String placement_str = { .length = (int)strlen(plabel), .chars = plabel };

                CLAY(CLAY_IDI("Card", i), {
                    .layout = {
                        .sizing = {
                            .width  = CLAY_SIZING_FIXED(card_width),
                            .height = CLAY_SIZING_FIXED(card_height)
                        },
                        .padding        = CLAY_PADDING_ALL((uint16_t)(16 * state_machine->ui_scale)),
                        .childGap       = (uint16_t)(10 * state_machine->ui_scale),
                        .childAlignment = {
                            .x = CLAY_ALIGN_X_CENTER,
                            .y = CLAY_ALIGN_Y_CENTER
                        },
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    },
                    .backgroundColor = { 20, 20, 45, 255 },
                    .cornerRadius = CLAY_CORNER_RADIUS(6),
                    .border = {
                        .color = accent,
                        .width = { 2, 2, 2, 2 }
                    }
                }) {
                    // Placement badge  e.g. "1st"
                    CLAY_TEXT(
                        placement_str,
                        CLAY_TEXT_CONFIG({
                            .textColor = accent,
                            .fontId    = FONT_BODY,
                            .fontSize  = (uint16_t)scaled_label_size
                        })
                    );

                    // Player label  e.g. "Player 1"
                    static const Clay_String player_labels[4] = {
                        CLAY_STRING("Player 1"),
                        CLAY_STRING("Player 2"),
                        CLAY_STRING("Player 3"),
                        CLAY_STRING("Player 4"),
                    };
                    CLAY_TEXT(
                        player_labels[i],
                        CLAY_TEXT_CONFIG({
                            .textColor = { 200, 200, 220, 255 },
                            .fontId    = FONT_BODY,
                            .fontSize  = (uint16_t)scaled_label_size,
                            .wrapMode  = CLAY_TEXT_WRAP_NONE
                        })
                    );

                    // Score value — convert int to string.
                    static char score_bufs[4][32];
                    snprintf(score_bufs[i], sizeof(score_bufs[i]), "%d", results[i].score);
                    Clay_String score_str = { .length = (int)strlen(score_bufs[i]), .chars = score_bufs[i] };

                    CLAY_TEXT(score_str, CLAY_TEXT_CONFIG({
                        .textColor = { 255, 255, 255, 255 },
                        .fontId    = FONT_BODY,
                        .fontSize  = (uint16_t)scaled_score_size
                    }));
                }
            }
        }

        CLAY(CLAY_ID("Spacer"), {
            .layout = {
                .sizing = {
                    .width  = CLAY_SIZING_GROW(0),
                    .height = CLAY_SIZING_GROW(0)
                }
            }
        }) {}

        // Return to Menu button
        CLAY(CLAY_ID("MenuButton"), {
            .layout = {
                .sizing = {
                    .width  = CLAY_SIZING_FIT(0),
                    .height = CLAY_SIZING_FIT(0)
                },
                .padding = { 16, 16, 8, 8 },
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
            CLAY_TEXT(CLAY_STRING("Main Menu"), CLAY_TEXT_CONFIG({
                .textColor = { 255, 255, 255, 255 },
                .fontId    = FONT_BODY,
                .fontSize  = (uint16_t)scaled_button_size
            }));
            Clay_OnHover(mainMenuButtonClicked, (void*)this);
        }

        // Hint text
        CLAY_TEXT(CLAY_STRING("Press SPACE or ENTER to return"), CLAY_TEXT_CONFIG({
            .textColor = { 120, 120, 150, 255 },
            .fontId    = FONT_BODY,
            .fontSize  = (uint16_t)(scaled_label_size * 0.7f)
        }));
    }

    state_machine->clay_render_commands = Clay_EndLayout();
}
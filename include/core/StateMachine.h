#ifndef __STATE_MACHINE__
#define __STATE_MACHINE__

#include <map>
#include <string>
#include <memory>
#include <SDL.h>
#include "clay/clay.h"
#include "clay/clay_renderer_SDL2.h"

#include "states/DefaultState.h"
#include "core/Input.h"

#define CANVAS_WIDTH  320
#define CANVAS_HEIGHT 240
#define MAX_FONTS 1

enum FontId {
    FONT_BODY = 0
};

class State;

class StateMachine {
public:
    StateMachine() = default;
    ~StateMachine();

    bool init();
    void transition(const std::string& state_name);

    State* current_state = nullptr;

    SDL_Window   *window   = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Event event;
    
    Input input_buffer;

    // Clay resources
    Clay_Arena clay_arena;
    Clay_RenderCommandArray clay_render_commands;
    SDL2_Font fonts[MAX_FONTS];

private:
    void init_states();
    void add(std::unique_ptr<State> state);
    void init_clay();
    void cleanup_clay();

    std::map<std::string, std::unique_ptr<State>> state_map;
    bool application_initialized = false;
    void* clay_memory = nullptr;
};

#endif /* __STATE_MACHINE__ */
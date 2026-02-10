#ifndef __STATE_MACHINE__
#define __STATE_MACHINE__

#include <map>
#include <string>
#include <memory>
#include <SDL.h>

#include "states/DefaultState.h"
#include "core/Input.h"

#define CANVAS_WIDTH  320
#define CANVAS_HEIGHT 240

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

private:
    void init_states();
    void add(std::unique_ptr<State> state);

    std::map<std::string, std::unique_ptr<State>> state_map;
    bool application_initialized = false;
};

#endif /* __STATE_MACHINE__ */
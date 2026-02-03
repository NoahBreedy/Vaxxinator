#ifndef __STATE_MACHINE__
#define __STATE_MACHINE__

#include <map>
#include <string>
#include <memory>
#include "DefaultState.h"

class State; 

class StateMachine {
public:
    StateMachine() = default;
    
    void init();
    void transition(const std::string& state_name);

    State* current_state = nullptr;

private:

    void add(std::unique_ptr<State> state);
    std::map<std::string, std::unique_ptr<State>> state_map;
};

#endif /* __STATE_MACHINE__ */
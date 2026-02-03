#ifndef __DEFAULT_STATE__
#define __DEFAULT_STATE__

#include <string>
#include "../StateMachine.h"

class StateMachine;

class State {
public:
    explicit State(std::string name, StateMachine* parent)
        : name(std::move(name)) {
            state_machine = parent;
        }

    virtual ~State() = default;

    virtual void render() = 0;
    virtual void update() = 0;
    virtual void enter()  = 0;
    virtual void exit()   = 0;

    const std::string& getName() const { return name; }

protected:
    std::string name;
    StateMachine* state_machine;
};

#endif /* __DEFAULT_STATE__ */
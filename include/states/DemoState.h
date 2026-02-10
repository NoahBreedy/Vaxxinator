#ifndef __DEMO_STATE__
#define __DEMO_STATE__

#include "DefaultState.h"

class DemoState : public State {
public:
    explicit DemoState(StateMachine* machine);

    void render() override;
    void update() override;
    void enter()  override;
    void exit()   override;
};

#endif /* __DEMO_STATE__ */
#ifndef __MAIN_MENU__
#define __MAIN_MENU__

#include "DefaultState.h"

class MainMenu : public State {
public:
    explicit MainMenu(StateMachine* machine);

    void render() override;
    void update() override;
    void enter()  override;
    void exit()   override;
};

#endif /* __MAIN_MENU__ */
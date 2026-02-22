#ifndef __MAIN_MENU__
#define __MAIN_MENU__

#include "DefaultState.h"
#include "clay/clay.h"
#include "clay/clay_renderer_SDL2.h"

class MainMenu : public State {
public:
    explicit MainMenu(StateMachine* machine);

    void render() override;
    void update() override;
    void enter()  override;
    void exit()   override;

private:
    void buildLayout();
    static void injectButtonClicked(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);
};

#endif /* __MAIN_MENU__ */
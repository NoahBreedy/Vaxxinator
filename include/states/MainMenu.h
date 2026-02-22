#ifndef __MAIN_MENU__
#define __MAIN_MENU__

#include <SDL2/SDL_image.h>
#include "DefaultState.h"
#include "clay/clay.h"
#include "clay/clay_renderer_SDL2.h"
#include "core/Utils.h"

class MainMenu;

struct SyringeData {
    int index;
    MainMenu* userData;
};

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
    static void onSyringeClicked(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData);

    SDL_Texture* syringe_sheet = nullptr;

    static constexpr SDL_Rect SYRINGE_EMPTY        = { 469, 62,  200, 450 };
    static constexpr SDL_Rect SYRINGE_EMPTY_HOVERED = { 835, 62,  200, 450 };
    static constexpr SDL_Rect SYRINGE_FULL          = { 469, 530, 200, 450 };
    static constexpr SDL_Rect SYRINGE_FULL_HOVERED  = { 835, 530, 200, 450 };

    bool syringe_loaded[4] = { false, false, false, false };

    SyringeData syringe_data[4];
};

#endif /* __MAIN_MENU__ */
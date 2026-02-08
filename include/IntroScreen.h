#ifndef __INTRO_SCREEN__
#define __INTRO_SCREEN__

#include <vector>
#include <SDL_ttf.h>

#include "DefaultState.h"

class IntroScreen : public State {
public:
    explicit IntroScreen(StateMachine* machine);

    void render() override;
    void update() override;
    void enter()  override;
    void exit()   override;

private:
    
    TTF_Font* font;
    std::vector<SDL_Texture*> text_textures;
    int elapsed_time;
    float scroll_speed;
    float line_spacing;
};

#endif /* __INTRO_SCREEN__ */
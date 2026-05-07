#ifndef __GAME_OVER__
#define __GAME_OVER__

#include <vector>
#include <string>
#include "DefaultState.h"
#include "clay/clay.h"
#include "clay/clay_renderer_SDL2.h"

struct PlayerResult {
    int score;
    int placement; // 1st, 2nd, etc.
};

class GameOver : public State {
public:
    explicit GameOver(StateMachine* machine);

    void render() override;
    void update() override;
    void enter()  override;
    void exit()   override;

private:
    void buildLayout();

    static void mainMenuButtonClicked(Clay_ElementId elementId, Clay_PointerData pointerData, void* userData);

    std::vector<PlayerResult> results; // sorted by score descending
};

#endif /* __GAME_OVER__ */
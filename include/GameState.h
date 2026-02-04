#ifndef __GAME_STATE__
#define __GAME_STATE__

#include "DefaultState.h"

class GameState : public State {
public:
    explicit GameState(StateMachine* machine);

    void render() override;
    void update() override;
    void enter()  override;
    void exit()   override;
};

#endif /* __GAME_STATE__ */
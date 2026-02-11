#ifndef __GAME_STATE__
#define __GAME_STATE__

#include <cstdint>

#include "DefaultState.h"
#include "core/open-simplex-noise.h"

class GameState : public State {
public:
    explicit GameState(StateMachine* machine);

    void render() override;
    void update() override;
    void enter()  override;
    void exit()   override;

private:
    /* This stuff is boiler plate needed for getting 
     * all of the pixel data out of a texture (hardware accelerated)
     */
    SDL_Texture *background_texture;
    uint32_t format;
    void *pixels;
    int pitch;

    osn_context *noise_ctx0;
    osn_context *noise_ctx1;
    uint32_t color_map[256];
    int strand_size;
    const int MOVEMENT_SCALER = 8;

    uint64_t frame_count;
};

#endif /* __GAME_STATE__ */
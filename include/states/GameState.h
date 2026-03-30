#ifndef __GAME_STATE__
#define __GAME_STATE__

#include <cstdint>
#include <vector>

#include "DefaultState.h"
#include "core/open-simplex-noise.h"
#include "entities/BloodCell.h"
#include "entities/Player.h"
#include "core/teenyat.h"

/* TeenyAT MMIO Ports */
#define SHOOT_DIR 0x9000
#define MOVE_DIR  0x9001
 
class GameState : public State {
public:
    explicit GameState(StateMachine* machine);

    void render() override;
    void update() override;
    void enter()  override;
    void exit()   override;

    /* The teenyAT bus read and bus write functions */
    static void bus_read(teenyat *t, tny_uword addr, tny_word *data, uint16_t *delay);
    static void bus_write(teenyat *t, tny_uword addr, tny_word data, uint16_t *delay);

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

    std::vector<BloodCell> cells;
    std::vector<Player> players;

};

#endif /* __GAME_STATE__ */
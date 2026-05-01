#ifndef __BLOOD_CELL__
#define __BLOOD_CELL__

#include <cstdint>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#include "core/Utils.h"
#include "graphics/Sprite.h"

#define NUM_BALLS 4
#define MOVEMENT_THRESHOLD 0.3
#define CHANGE_MOVEMENT_THRESHOLD 0.9

class BloodCell  {
public:
    explicit BloodCell(uint16_t _x, uint16_t _y, uint16_t _radius, bool is_white, SDL_Renderer *renderer);

    ~BloodCell() = default;

    void render(SDL_Renderer *renderer);
    void update();

    int getX() const { return (int)positions[0].x; }
    int getY() const { return (int)positions[0].y; }
    SDL_Rect getHitbox() const;

private:
    Vec2f positions[NUM_BALLS];
    Vec2f velocities[NUM_BALLS];
    RGBA cell_color;
    int radius;
    Sprite *sprite;
};

#endif /* __BLOOD_CELL__ */
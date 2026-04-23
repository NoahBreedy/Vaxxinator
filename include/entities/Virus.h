#ifndef __VIRUS__
#define __VIRUS__

#include <cstdint>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#include "core/Utils.h"

class Virus {
public:
    explicit Virus(uint16_t _x, uint16_t _y, uint16_t _size);
    virtual ~Virus() = default;

    virtual void render(SDL_Renderer *renderer) = 0;
    virtual void update() = 0;
    void wrapPosition(int canvasWidth, int canvasHeight);
    
    int getX() const { return (int)centerPosition.x; }
    int getY() const { return (int)centerPosition.y; }

protected:
    Vec2f position; // Top-left corner of the virus (used for triangles and squares)
    Vec2f centerPosition; // Center of the virus (used for circles and for calculating direction to player)
    Vec2f velocity;
    uint16_t size;
    RGBA color;
};

class CircleVirus : public Virus {
public:
    explicit CircleVirus(uint16_t _x, uint16_t _y, uint16_t _size);
    ~CircleVirus() = default;

    void render(SDL_Renderer *renderer) override;
    void update() override;
};

class TriangleVirus : public Virus {
public:
    explicit TriangleVirus(uint16_t _x, uint16_t _y, uint16_t _size);
    ~TriangleVirus() = default;

    void render(SDL_Renderer *renderer) override;
    void update() override;
};

class SquareVirus : public Virus {
public:
    explicit SquareVirus(uint16_t _x, uint16_t _y, uint16_t _size);
    ~SquareVirus() = default;

    void render(SDL_Renderer *renderer) override;
    void update() override;
};

#endif /* __VIRUS__ */

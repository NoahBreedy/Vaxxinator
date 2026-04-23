#include "entities/Virus.h"

// Virus Base Class Constructor
Virus::Virus(uint16_t _x, uint16_t _y, uint16_t _size) 
    : size(_size) {
    position.x = _x;
    position.y = _y;
    centerPosition.x = _x;
    centerPosition.y = _y;
    velocity.x = rand_float(-0.5f, 0.5f);
    velocity.y = rand_float(-0.5f, 0.5f);
}

void Virus::wrapPosition(int canvasWidth, int canvasHeight) {
    if (position.x < -size) position.x = canvasWidth + size;
    if (position.x > canvasWidth + size) position.x = -size;
    if (position.y < -size) position.y = canvasHeight + size;
    if (position.y > canvasHeight + size) position.y = -size;
}

// CircleVirus Implementation
CircleVirus::CircleVirus(uint16_t _x, uint16_t _y, uint16_t _size) 
    : Virus(_x, _y, _size) {
    color = RGBA{255, 100, 100, 255}; // Red color
}

void CircleVirus::render(SDL_Renderer *renderer) {
    filledCircleRGBA(renderer, 
                     (int)position.x, 
                     (int)position.y, 
                     size, 
                     color.r, color.g, color.b, color.a);
    
    circleRGBA(renderer, 
               (int)position.x, 
               (int)position.y, 
               size, 
               0, 0, 0, 0); // black outline
}

void CircleVirus::update() {
    position.x += velocity.x;
    position.y += velocity.y;

    centerPosition.x = position.x;
    centerPosition.y = position.y;
}

// TriangleVirus Implementation
TriangleVirus::TriangleVirus(uint16_t _x, uint16_t _y, uint16_t _size) 
    : Virus(_x, _y, _size) {
    color = RGBA{100, 255, 100, 255}; // Green color
}

void TriangleVirus::render(SDL_Renderer *renderer) {
    int16_t x1 = position.x;
    int16_t y1 = position.y - size;
    int16_t x2 = position.x - size;
    int16_t y2 = position.y + size;
    int16_t x3 = position.x + size;
    int16_t y3 = position.y + size;
    
    filledTrigonRGBA(renderer, x1, y1, x2, y2, x3, y3, 
                     color.r, color.g, color.b, color.a);
    
    trigonRGBA(renderer, x1, y1, x2, y2, x3, y3, 
               0, 0, 0, 0); // Black outline
}

void TriangleVirus::update() {
    // Top vertex
    position.x += velocity.x;
    position.y += velocity.y;
    // Center is 1/3 of the way down from the top vertex for triangles
    centerPosition.x = position.x;
    centerPosition.y = position.y + (size * 2) / 3; // Center of mass for triangle
}

// SquareVirus Implementation
SquareVirus::SquareVirus(uint16_t _x, uint16_t _y, uint16_t _size) 
    : Virus(_x, _y, _size) {
    color = RGBA{100, 100, 255, 255}; // Blue color
}

void SquareVirus::render(SDL_Renderer *renderer) {
    int16_t x1 = position.x - size;
    int16_t y1 = position.y - size;
    int16_t x2 = position.x + size;
    int16_t y2 = position.y + size;
    
    boxRGBA(renderer, x1, y1, x2, y2, 
            color.r, color.g, color.b, color.a);
    
    rectangleRGBA(renderer, x1, y1, x2, y2, 
                  0, 0, 0, 0); // Black outline
}

void SquareVirus::update() {
    position.x += velocity.x;
    position.y += velocity.y;
    // Center is at the middle of the square (size away from the top-left corner)
    centerPosition.x = position.x + size;
    centerPosition.y = position.y + size;
}

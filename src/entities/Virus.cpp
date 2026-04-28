#include "entities/Virus.h"

// Virus Base Class Constructor
Virus::Virus(uint16_t _x, uint16_t _y, uint16_t _size, SDL_Renderer *renderer) 
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
CircleVirus::CircleVirus(uint16_t _x, uint16_t _y, uint16_t _size, SDL_Renderer *renderer) 
    : Virus(_x, _y, _size, renderer) {
    color = RGBA{133, 133, 133, 255}; // Red color
    sprite = new Sprite(renderer);
    sprite->setScale(1);
    // Idle
    sprite->addAnimation("idle", "assets/sprites/cells/cell_face_1.png",  16, 16, 8, -1, true);
    sprite->setBaseAnimation("idle", true);
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

     sprite->render();
}

void CircleVirus::update() {
    position.x += velocity.x;
    position.y += velocity.y;

    centerPosition.x = position.x;
    centerPosition.y = position.y;

    sprite->setPosition(centerPosition.x - size + (rand() % 3), centerPosition.y - size + (rand() % 3));
    sprite->update();
}

// TriangleVirus Implementation
TriangleVirus::TriangleVirus(uint16_t _x, uint16_t _y, uint16_t _size, SDL_Renderer *renderer) 
    : Virus(_x, _y, _size, renderer) {
    color = RGBA{100, 255, 100, 255}; // Green color

    sprite = new Sprite(renderer);
    sprite->setScale(1);
    // Idle
    sprite->addAnimation("idle", "assets/sprites/cells/cell_face_1.png",  16, 16, 8, -1, true);
    sprite->setBaseAnimation("idle", true);
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

     sprite->render();
}

void TriangleVirus::update() {
    // Top vertex
    position.x += velocity.x;
    position.y += velocity.y;
    // Center is 1/3 of the way down from the top vertex for triangles
    centerPosition.x = position.x;
    centerPosition.y = position.y + (size * 2) / 3; // Center of mass for triangle

    sprite->setPosition(centerPosition.x - 8, centerPosition.y - (size));
    sprite->update();
}

// SquareVirus Implementation
SquareVirus::SquareVirus(uint16_t _x, uint16_t _y, uint16_t _size, SDL_Renderer *renderer) 
    : Virus(_x, _y, _size, renderer) {
    color = RGBA{75, 16, 92, 255}; // violet color

    sprite = new Sprite(renderer);
    sprite->setScale(1);
    // Idle
    sprite->addAnimation("idle", "assets/sprites/cells/cell_face_1.png",  16, 16, 8, -1, true);
    sprite->setBaseAnimation("idle", true);
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
    
    sprite->render();
}

void SquareVirus::update() {
    position.x += velocity.x;
    position.y += velocity.y;
    // Center is at the middle of the square (size away from the top-left corner)
    centerPosition.x = position.x + size;
    centerPosition.y = position.y + size;

    sprite->setPosition(centerPosition.x - (size + 8), centerPosition.y - (size + 8));
    sprite->update();
}

SDL_Rect CircleVirus::getHitbox() const {
    int x = (int)position.x;
    int y = (int)position.y;
    int expandedSize = (int)(size * 1.25);
    return {x - expandedSize, y - expandedSize, expandedSize * 2, expandedSize * 2};
}

SDL_Rect TriangleVirus::getHitbox() const {
    int x = (int)position.x;
    int y = (int)position.y;
    // Bounding box for triangle: top vertex at (x, y-size), base vertices at (x±size, y+size)
    return {x - (int)size, y - (int)size, size * 2, size * 2};
}

SDL_Rect SquareVirus::getHitbox() const {
    int x = (int)position.x;
    int y = (int)position.y;
    // Square spans from (x-size, y-size) to (x+size, y+size)
    int expandedSize = (int)(size * 1.25);
    return {x - expandedSize, y - expandedSize, expandedSize * 2, expandedSize * 2};
}

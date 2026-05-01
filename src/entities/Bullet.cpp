#include "entities/Bullet.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <cmath>

Bullet::Bullet(float startX, float startY, float dirX, float dirY, float speed)
    : x(startX), y(startY) {
    velocityX = dirX * speed;
    velocityY = dirY * speed;
}

void Bullet::update() {
    x += velocityX;
    y += velocityY;
}

void Bullet::render(SDL_Renderer* renderer) {
    // Calculate endpoint of the line
    int x1 = (int)x;
    int y1 = (int)y;
    int x2 = (int)(x + velocityX * BULLET_LENGTH / 5.0f);
    int y2 = (int)(y + velocityY * BULLET_LENGTH / 5.0f);
    
    // Draw white line for bullet
    lineRGBA(renderer, x1, y1, x2, y2, 255, 255, 255, 255);
}

bool Bullet::isOutOfBounds(int canvasWidth, int canvasHeight) const {
    return x < -50 || x > canvasWidth + 50 || y < -50 || y > canvasHeight + 50;
}

SDL_Rect Bullet::getHitbox() const {
    // Create a small square hitbox around the bullet center
    const int HITBOX_SIZE = 4;
    return {(int)x - HITBOX_SIZE / 2, (int)y - HITBOX_SIZE / 2, HITBOX_SIZE, HITBOX_SIZE};
}
